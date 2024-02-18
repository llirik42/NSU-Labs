package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.controller.DialogWindowController;
import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.context.ContextAction;
import ru.nsu.kondrenko.model.context.ContextListener;
import ru.nsu.kondrenko.model.context.ContextTools;

import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.ComponentListener;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.util.EnumMap;
import java.util.Map;
import java.util.function.Consumer;

public class View implements ContextListener {
    private static final String FILE_OPENING_ERROR_MESSAGE = "Cannot open file";
    private static final String FILE_SAVING_ERROR_MESSAGE = "Cannot save file";
    private static final String COLOR_SELECTION_DIALOG_TITLE = "Color";
    private static final String THICKNESS_SELECTION_DIALOG_TITLE = "Thickness";
    private static final String NUMBER_OF_VERTICES_SELECTION_DIALOG_TITLE = "Number of vertices";
    private static final String RADIUS_SELECTION_DIALOG_TITLE = "Radius";
    private static final String ROTATION_DIALOG_TITLE = "Rotation";
    private static final String ERROR_DIALOG_TITLE = "Error";
    private static final String HELP_DIALOG_TITLE = "Help";
    private static final String ABOUT_DIALOG_TITLE = "About";

    private final Map<ContextAction, Consumer<Context>> contextStateChangeHandlers = new EnumMap<>(ContextAction.class);
    private final DialogWindowController dialogWindowController;
    private final JFileChooser imagesOpeningFileChoose;
    private final JFileChooser pngSavingFileChooser;
    private final JTextPane helpTextPane;
    private final JTextPane aboutTextPane;
    private final MenuArea menuArea;
    private final ToolsArea toolsArea;
    private final DrawingArea drawingArea;
    private final JFrame frame;
    private final int minThickness;
    private final int maxThickness;
    private final int minNumberOfVertices;
    private final int maxNumberOfVertices;
    private final int minRadius;
    private final int maxRadius;
    private final int minRotation;
    private final int maxRotation;

    public View(String viewName,
                int minFrameWidth,
                int minFrameHeight,
                int minThickness,
                int maxThickness,
                int minNumberOfVertices,
                int maxNumberOfVertices,
                int minRadius,
                int maxRadius,
                int minRotation,
                int maxRotation,
                String[] supportedImageFormats,
                ActionListener buttonsListener,
                ActionListener filesActionsListener,
                MouseListener mouseListener,
                ComponentListener drawingAreaResizingListener,
                DialogWindowController dialogWindowController,
                ToolsIconsSupplier toolsIconsSupplier
    ) {
        contextStateChangeHandlers.put(ContextAction.IDLE, View.this::onIdle);
        contextStateChangeHandlers.put(ContextAction.REPAINT, View.this::onRepainting);
        contextStateChangeHandlers.put(ContextAction.OPEN_FILE, View.this::onOpeningFile);
        contextStateChangeHandlers.put(ContextAction.SAVE_FILE, View.this::onSavingFile);
        contextStateChangeHandlers.put(ContextAction.EXIT, View.this::onExiting);
        contextStateChangeHandlers.put(ContextAction.SELECT_COLOR, View.this::onChoosingColor);
        contextStateChangeHandlers.put(ContextAction.SELECT_THICKNESS, View.this::onSelectingThickness);
        contextStateChangeHandlers.put(ContextAction.SELECT_NUMBER_OF_VERTICES, View.this::onSelectingNumberOfVertices);
        contextStateChangeHandlers.put(ContextAction.SELECT_RADIUS, View.this::onSelectingRadius);
        contextStateChangeHandlers.put(ContextAction.SELECT_ROTATION, View.this::onSelectingRotation);
        contextStateChangeHandlers.put(ContextAction.HANDLE_ERROR, View.this::onError);
        contextStateChangeHandlers.put(ContextAction.DISPLAY_HELP, View.this::onDisplayingHelp);
        contextStateChangeHandlers.put(ContextAction.DISPLAY_ABOUT, View.this::onDisplayingAbout);

        this.dialogWindowController = dialogWindowController;
        this.imagesOpeningFileChoose = createImagesOpeningChooser(filesActionsListener, supportedImageFormats);
        this.pngSavingFileChooser = createPNGSavingChooser(filesActionsListener);
        this.helpTextPane = createHelpTextPane();
        this.aboutTextPane = createAboutTextArea();
        this.menuArea = new MenuArea(buttonsListener);
        this.toolsArea = new ToolsArea(toolsIconsSupplier, buttonsListener);
        this.drawingArea = new DrawingArea(mouseListener, drawingAreaResizingListener);
        this.minThickness = minThickness;
        this.maxThickness = maxThickness;
        this.minNumberOfVertices = minNumberOfVertices;
        this.maxNumberOfVertices = maxNumberOfVertices;
        this.minRadius = minRadius;
        this.maxRadius = maxRadius;
        this.minRotation = minRotation;
        this.maxRotation = maxRotation;
        this.frame = createFrame(viewName, minFrameWidth, minFrameHeight);
    }

    private static JFileChooser createImagesOpeningChooser(ActionListener actionListener, String[] supportedImageFormats) {
        final FileFilter filesOpeningFilter = new FileNameExtensionFilter("Images", supportedImageFormats);
        final JFileChooser result = new JFileChooser();
        initFileChooser(result, filesOpeningFilter, actionListener);
        return result;
    }

    private static JFileChooser createPNGSavingChooser(ActionListener actionListener) {
        final FileFilter filesSavingFilter = new FileSavingFilter("PNG");
        final JFileChooser result = new JFileChooser();
        initFileChooser(result, filesSavingFilter, actionListener);
        return result;
    }

    private static void initFileChooser(JFileChooser fileChooser, FileFilter fileFilter, ActionListener actionListener) {
        fileChooser.setAcceptAllFileFilterUsed(false);
        fileChooser.addChoosableFileFilter(fileFilter);
        fileChooser.addActionListener(actionListener);
    }

    private static JTextPane createHelpTextPane() {
        final JTextPane result = createHelpAboutTextPane();
        result.setText(getHelpText());
        return result;
    }

    private static JTextPane createAboutTextArea() {
        final JTextPane result = createHelpAboutTextPane();
        result.setText(getAboutText());
        return result;
    }

    private static String getHelpText() {
        return """
                <html>
                    <p><b><i>Simple Paint</i></b> represents a simple program for drawing simple shapes and filling, it has 3 areas</p>
                    <ul>
                    <li><b>Canvas</b> — is a working area where all drawings and fillings are visible</li>
                    <li><b>Menu</b> contains main functionality and consists of</li>
                    <ul>
                        <li>
                            <i>File</i>
                            <ul>
                                <li><u>Open</u> — opens file, supported formats are: PNG, JPEG, BMP, GIF</li>
                                <li><u>Save</u> — saves file as PNG-image</li>
                                <li><u>Exit</u> — exits program without any saving</li>
                            </ul>
                        </li>
                        <li>
                            <i>Edit</i>
                            <ul>
                                <li><u>Line</u> — draws line between 2 points of mouse-pressing</li>
                                <li><u>Polygon</u> — draws polygon where mouse is pressed</li>
                                <li><u>Star</u> — draws star where mouse is pressed, inner radius is equal to outer radius divided by 2</li>
                                <li><u>Fill</u> — fills 4-connected area that contains point of mouse-pressing</li>
                                <li><u>Clear</u> — clears <b>Canvas</b></li>
                                <li><u>Select color</u> — opens dialog window to select current color</li>
                                <li><u>Select thickness</u> — opens dialog window to select current thickness</li>
                                <li><u>Select number of vertices</u> — opens dialog window to select current number of vertices</li>
                                <li><u>Select rotation</u> — opens dialog window to select current rotation</li>
                                <li><u>Select radius</u> — opens dialog window to select current radius</li>
                            </ul>
                        </li>
                        <li>
                            <i>Info</i>
                            <ul>
                                <li><u>Help</u> — shows help</li>
                                <li><u>About</u> — shows about</li>
                            </ul>
                        </li>
                    </ul>
                    <li><b>Tools</b> contains all functionality of <b>Menu</b> and adds additional tools (such as <i>change color to red</i>)</li>
                    </ul>
                    <p>There are <b>parameters</b> that are defined while the program is running</p>
                    <ul>
                        <li><i>color</i> — which color to use during drawing shapes and filling</li>
                        <li><i>thickness</i> — thickness of shapes</li>
                        <li><i>number of vertices</i> — number of vertices for shapes (such as polygons and stars)</li>
                        <li><i>rotation</i> — how many degrees will the figure be rotated clockwise</li>
                        <li><i>radius</i> — determines radius of shapes, for polygons and stars it means radius of the circumcircle</li>
                    </ul>
               </html>""";
    }

    private static String getAboutText() {
        return """
        <html>
            <p><i><b>Simple Paint</b></i></p>
            <p>Created by Kondrenko Kirill in February 2024 as task for the course "engineering and computer graphics" in Novosibirsk State University</p>
        </html>""";
    }

    private static JTextPane createHelpAboutTextPane() {
        final JTextPane result = new JTextPane();
        result.setEditable(false);
        result.setBackground(null);
        result.setContentType("text/html");
        return result;
    }
    
    private static String getIncorrectValueMessage(int minValue, int maxValue) {
        return String.format("Incorrect value! It must be in [%d, %d]", minValue, maxValue);
    }

    public void show() {
        frame.setVisible(true);
    }

    @Override
    public void onContextActionChange(Context context) {
        contextStateChangeHandlers.get(context.getAction()).accept(context);
    }

    @Override
    public void onContextToolChange(Context context) {
        final ContextTools tool = context.getTool();

        toolsArea.unselectAll();

        switch (tool) {
            case DRAW_LINE -> {
                this.menuArea.setDrawLineSelected();
                this.toolsArea.setDrawLineSelected();
            }

            case DRAW_POLYGON -> {
                this.menuArea.setDrawPolygonSelected();
                this.toolsArea.setDrawPolygonSelected();
            }

            case DRAW_STAR -> {
                this.menuArea.setDrawStarSelected();
                this.toolsArea.setDrawStarSelected();
            }

            case FILL -> {
                this.menuArea.setFillSelected();
                this.toolsArea.setFillSelected();
            }
        }
    }

    private void onIdle(Context context) {
        // Method is empty because we don't want to do anything with action "IDLE" of context
    }

    private void onRepainting(Context context) {
        repaint(context.getImage());
    }

    private void onOpeningFile(Context context) {
        final int code = imagesOpeningFileChoose.showOpenDialog(frame);

        if (code == JFileChooser.ERROR_OPTION) {
            showError(FILE_OPENING_ERROR_MESSAGE);
        }
    }

    private void onSavingFile(Context context) {
        final int code = pngSavingFileChooser.showSaveDialog(frame);

        if (code == JFileChooser.ERROR_OPTION) {
            showError(FILE_SAVING_ERROR_MESSAGE);
        }
    }

    private void onExiting(Context context) {
        frame.setVisible(false);
        frame.dispose();
    }

    private void onChoosingColor(Context context) {
        final Color newContextColor = showChooseColorDialogWindow(context.getColor());

        if (newContextColor != null) {
            context.setColor(newContextColor);
        }
    }

    private void onSelectingThickness(Context context) {
        context.setThickness(showSelectThicknessDialogWindow(context.getThickness()));
    }

    private void onSelectingNumberOfVertices(Context context) {
        context.setNumberOfVertices(showSelectNumberOfVerticesDialogWindow(context.getNumberOfVertices()));
    }

    private void onSelectingRadius(Context context) {
        context.setRadius(showSelectRadiusDialogWindow(context.getRadius()));
    }

    private void onSelectingRotation(Context context) {
        context.setRotation(showSelectRotationDialogWindow(context.getRotation()));
    }

    private void onError(Context context) {
        showError(context.getErrorMessage());
    }

    private void onDisplayingHelp(Context context) {
        JOptionPane.showMessageDialog(
                null,
                helpTextPane,
                HELP_DIALOG_TITLE,
                JOptionPane.INFORMATION_MESSAGE
        );
    }

    private void onDisplayingAbout(Context context) {
        JOptionPane.showMessageDialog(
                null,
                aboutTextPane,
                ABOUT_DIALOG_TITLE,
                JOptionPane.INFORMATION_MESSAGE
        );
    }

    private Color showChooseColorDialogWindow(Color startColor) {
        return JColorChooser.showDialog(null, COLOR_SELECTION_DIALOG_TITLE, startColor);
    }

    private int showSelectThicknessDialogWindow(int startThickness) {
        return selectInteger(THICKNESS_SELECTION_DIALOG_TITLE, minThickness, maxThickness, startThickness);
    }

    private int showSelectNumberOfVerticesDialogWindow(int startNumberOfVertices) {
        return selectInteger(NUMBER_OF_VERTICES_SELECTION_DIALOG_TITLE, minNumberOfVertices, maxNumberOfVertices, startNumberOfVertices);
    }

    private int showSelectRadiusDialogWindow(int startRadius) {
        return selectInteger(RADIUS_SELECTION_DIALOG_TITLE, minRadius, maxRadius, startRadius);
    }

    private int showSelectRotationDialogWindow(int startRotation) {
        return selectInteger(ROTATION_DIALOG_TITLE, minRotation, maxRotation, startRotation);
    }

    private int selectInteger(String message, int minValue, int maxValue, int startValue) {
        final String incorrectValueMessage = getIncorrectValueMessage(minValue, maxValue);
        final JSlider slider = new JSlider(minValue, maxValue, startValue);
        final JTextField textField = new JTextField(Integer.toString(startValue));

        dialogWindowController.reset();
        dialogWindowController.setSlider(slider);
        dialogWindowController.setTextField(textField);
        slider.addChangeListener(dialogWindowController);
        textField.getDocument().addDocumentListener(dialogWindowController);
        textField.addActionListener(dialogWindowController);

        final JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(2, 1));
        panel.add(textField);
        panel.add(slider);

        JOptionPane.showConfirmDialog(null,
                panel,
                message,
                JOptionPane.OK_CANCEL_OPTION,
                JOptionPane.PLAIN_MESSAGE
        );

        if (dialogWindowController.hasError()) {
            showError(incorrectValueMessage);
            return startValue;
        }

        return dialogWindowController.hasValue() ? dialogWindowController.getValue() : startValue;
    }

    private void showError(String errorMessage) {
        JOptionPane.showMessageDialog(
                null,
                errorMessage,
                ERROR_DIALOG_TITLE,
                JOptionPane.ERROR_MESSAGE
        );
    }

    private void repaint(BufferedImage image) {
        drawingArea.resizeSoftly(image.getWidth(), image.getHeight());
        drawingArea.setImage(image);
        drawingArea.repaint();
        drawingArea.revalidate();
        frame.pack();
    }

    private JFrame createFrame(String frameName, int minFrameWidth, int minFrameHeight) {
        final JFrame result = new JFrame(frameName);
        result.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
        result.setMinimumSize(new Dimension(minFrameWidth, minFrameHeight));

        final JScrollPane scrollPane = new JScrollPane(drawingArea);
        scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        result.add(scrollPane, BorderLayout.CENTER);
        result.setJMenuBar(menuArea.getMenuBar());
        result.add(toolsArea, BorderLayout.NORTH);
        result.pack();

        return result;
    }
}
