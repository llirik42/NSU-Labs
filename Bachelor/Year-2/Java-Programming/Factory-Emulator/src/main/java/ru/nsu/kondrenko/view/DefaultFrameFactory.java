package ru.nsu.kondrenko.view;

import ru.nsu.kondrenko.Main;
import ru.nsu.kondrenko.model.parsing.config.Config;
import ru.nsu.kondrenko.model.parsing.limits.Limits;

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.event.ChangeListener;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.awt.event.WindowListener;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;

public class DefaultFrameFactory implements AbstractFrameFactory {
    private static final String ICON_NAME = "default_icon.png";

    private static final Font LABELS_FONT = new Font(Font.SERIF, Font.PLAIN, 30);
    private static final Font TABLE_FONT = new Font(Font.SERIF, Font.PLAIN, 18);
    private static final Font SLIDER_TITLE_FONT = new Font(Font.SERIF, Font.PLAIN, 20);
    private static final Font SLIDER_UNITS_FONT = new Font(Font.SERIF, Font.PLAIN, 12);

    private static final Dimension FRAME_DIMENSION = new Dimension(1500, 600);

    private static final DefaultTableCellRenderer cellRenderer = new DefaultTableCellRenderer();

    private static final int TABLE_HEADER_HEIGHT = 60;
    private static final int TABLE_ROW_HEIGHT = 75;
    private static final int SECTION_TITLE_WIDTH = 300;
    private static final int SLIDERS_STRUT = 60;

    private static final String DEFAULT_TABLE_VALUE = "0";

    private static final int SLIDER_TICKS_COUNT = 3;

    private static final String TOTAL_SECTION_TITLE = "Total";
    private static final String STORED_SECTION_TITLE = "Stored";
    private static final String WORKERS_COUNT_SECTION_TITLE = "Number";
    private static final String WORK_TIME_SECTION_TITLE = "Work time (ms)";
    private static final String SECTION_TITLE_OFFSET = "  ";

    private static final String[] SLIDER_TITLES = new String[]{
            "Body suppliers",
            "Engine suppliers",
            "Accessory suppliers",
            "Factory workers",
            "Dealers"
    };

    private static final String[] TOTAL_TABLE_HEADER_COLUMNS = new String[]{
            "Bodies",
            "Engines",
            "Accessories",
            "Assembled",
            "Sold"
    };

    private static final String[] STORED_TABLE_HEADER_COLUMNS = new String[]{
            "Bodies",
            "Engines",
            "Accessories",
            "Cars",
            "Tasks"
    };

    static {
        cellRenderer.setHorizontalAlignment(SwingConstants.CENTER);
    }

    private static JPanel createTableSection(JLabel labelWithSectionTitle, JTable table) {
        final JPanel section = new JPanel();
        section.setLayout(new BoxLayout(section, BoxLayout.X_AXIS));
        section.add(labelWithSectionTitle);
        section.add(new JScrollPane(table));
        return section;
    }

    private static JPanel createSlidersSection(JLabel labelWithSectionTitle, JSlider[] sliders) {
        final JPanel section = new JPanel();
        section.setLayout(new BoxLayout(section, BoxLayout.X_AXIS));
        section.add(labelWithSectionTitle);
        addSlidersToPanel(section, sliders);
        return section;
    }

    private static JTable createEmptyTable(String[] columnsHeaders) {
        final Object[][] data = new Object[1][columnsHeaders.length];

        for (int i = 0; i < columnsHeaders.length; i++) {
            data[0][i] = DEFAULT_TABLE_VALUE;
        }

        final JTable ret = new JTable(data, columnsHeaders) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };

        ret.getTableHeader().setReorderingAllowed(false);
        ret.getTableHeader().setFont(TABLE_FONT);
        ret.setFont(TABLE_FONT);
        ret.setFillsViewportHeight(true);
        ret.setRowHeight(TABLE_ROW_HEIGHT);
        ret.getTableHeader().setPreferredSize(new Dimension(0, TABLE_HEADER_HEIGHT));

        for (int i = 0; i < columnsHeaders.length; i++) {
            ret.getColumnModel().getColumn(i).setCellRenderer(cellRenderer);
        }

        return ret;
    }

    private static JSlider[] createProductivitySliders(Config config,
                                                       Limits limits,
                                                       ChangeListener bodySupplyTimeListener,
                                                       ChangeListener engineSupplyTimeListener,
                                                       ChangeListener accessorySupplyTimeListener,
                                                       ChangeListener carAssemblingTimeListener,
                                                       ChangeListener carRequestTimeListener) {

        final JSlider bodySupplyTimeSlider = createHorizontalSlider(
                bodySupplyTimeListener,
                limits.minBodySupplyTimeMs(),
                limits.maxBodySupplyTimeMs(),
                config.bodySupplyTimeMs()
        );
        final JSlider engineSupplyTimeSlider = createHorizontalSlider(
                engineSupplyTimeListener,
                limits.minEngineSupplyTimeMs(),
                limits.maxEngineSupplyTimeMs(),
                config.engineSupplyTimeMs()
        );
        final JSlider accessorySupplyTimeSlider = createHorizontalSlider(
                accessorySupplyTimeListener,
                limits.minAccessorySupplyTimeMs(),
                limits.maxAccessorySupplyTimeMs(),
                config.accessorySupplyTimeMs()
        );
        final JSlider carAssemblingTimeSlider = createHorizontalSlider(
                carAssemblingTimeListener,
                limits.minCarAssemblingTimeMs(),
                limits.maxCarAssemblingTimeMs(),
                config.carAssemblingTimeMs()
        );
        final JSlider carRequestTimeSlider = createHorizontalSlider(
                carRequestTimeListener,
                limits.minCarRequestTimeMs(),
                limits.maxCarRequestTimeMs(),
                config.carRequestTimeMs()
        );

        return new JSlider[]{
                bodySupplyTimeSlider,
                engineSupplyTimeSlider,
                accessorySupplyTimeSlider,
                carAssemblingTimeSlider,
                carRequestTimeSlider
        };
    }

    private static JSlider[] createThreadNumberSliders(Config config,
                                                       Limits limits,
                                                       ChangeListener bodySuppliersCountListener,
                                                       ChangeListener engineSuppliersCountListener,
                                                       ChangeListener accessorySuppliersCountListener,
                                                       ChangeListener workersCountListener,
                                                       ChangeListener dealersCountListener) {

        final JSlider bodySuppliersCountSlider = createHorizontalSlider(
                bodySuppliersCountListener,
                limits.minBodySuppliersCount(),
                limits.maxBodySuppliersCount(),
                config.bodySuppliersCount()
        );
        final JSlider engineSuppliersCountSlider = createHorizontalSlider(
                engineSuppliersCountListener,
                limits.minEngineSuppliersCount(),
                limits.maxEngineSuppliersCount(),
                config.engineSuppliersCount()
        );
        final JSlider accessorySuppliersCountSlider = createHorizontalSlider(
                accessorySuppliersCountListener,
                limits.minAccessorySuppliersCount(),
                limits.maxAccessorySuppliersCount(),
                config.accessorySuppliersCount()
        );
        final JSlider workersCountSlider = createHorizontalSlider(
                workersCountListener,
                limits.minWorkersCount(),
                limits.maxWorkersCount(),
                config.workersCount()
        );
        final JSlider dealersCountSlider = createHorizontalSlider(
                dealersCountListener,
                limits.minDealersCount(),
                limits.maxDealersCount(),
                config.dealersCount()
        );

        return new JSlider[]{
                bodySuppliersCountSlider,
                engineSuppliersCountSlider,
                accessorySuppliersCountSlider,
                workersCountSlider,
                dealersCountSlider
        };
    }

    private static void addSlidersToPanel(JPanel jPanel, JSlider[] sliders) {
        for (int i = 0; i < sliders.length; i++) {
            final JPanel tmp = new JPanel();
            tmp.setLayout(new GridLayout(2, 1));

            final JLabel jLabel = new JLabel(SLIDER_TITLES[i], SwingConstants.CENTER);
            jLabel.setFont(SLIDER_TITLE_FONT);

            tmp.add(jLabel, Component.RIGHT_ALIGNMENT);
            tmp.add(sliders[i]);

            jPanel.add(tmp);
            jPanel.add(Box.createHorizontalStrut(SLIDERS_STRUT));
        }
    }

    private static JSlider createHorizontalSlider(ChangeListener changeListener,
                                                  int minValue,
                                                  int maxValue,
                                                  int startValue) {

        final JSlider ret = new JSlider();
        configureSlider(ret, changeListener, minValue, maxValue, startValue);
        return ret;
    }

    private static void configureSlider(JSlider slider,
                                        ChangeListener changeListener,
                                        int minValue,
                                        int maxValue,
                                        int startValue) {

        final int step = (maxValue - minValue) / SLIDER_TICKS_COUNT;
        slider.setMinimum(minValue);
        slider.setMaximum(maxValue);
        slider.setValue(startValue);
        slider.setOrientation(SwingConstants.HORIZONTAL);
        slider.setPaintTicks(true);
        slider.setPaintLabels(true);
        slider.setMajorTickSpacing(step);
        slider.setMinorTickSpacing(step);
        slider.setFont(SLIDER_UNITS_FONT);
        slider.addChangeListener(changeListener);
    }

    private static JLabel createLabelWithSectionTitle(String sectionTitle) {
        final JLabel ret = new JLabel(sectionTitle);
        ret.setFont(LABELS_FONT);
        ret.setPreferredSize(new Dimension(SECTION_TITLE_WIDTH, 0));
        ret.setMinimumSize(new Dimension(SECTION_TITLE_WIDTH, 0));
        return ret;
    }

    private static void loadApplicationIcon(JFrame frame) {
        try (InputStream inputStream = Main.class.getClassLoader().getResourceAsStream(ICON_NAME)) {
            if (inputStream != null) {
                final BufferedImage bufferedImage = ImageIO.read(inputStream);
                final Image image = Toolkit.getDefaultToolkit().createImage(bufferedImage.getSource());
                frame.setIconImage(image);
            }
        } catch (IOException ignored) {
        }
    }

    @Override
    public Frame createMainFrame(Config config,
                                 Limits limits,
                                 ChangeListener bodySuppliersCountListener,
                                 ChangeListener engineSuppliersCountListener,
                                 ChangeListener accessorySuppliersCountListener,
                                 ChangeListener workersCountListener,
                                 ChangeListener dealersCountListener,
                                 ChangeListener bodySupplyTimeListener,
                                 ChangeListener engineSupplyTimeListener,
                                 ChangeListener accessorySupplyTimeListener,
                                 ChangeListener carAssemblingTimeListener,
                                 ChangeListener carRequestTimeListener,
                                 WindowListener windowListener) {

        final JTable suppliedTable = createEmptyTable(TOTAL_TABLE_HEADER_COLUMNS);
        final JTable storedTable = createEmptyTable(STORED_TABLE_HEADER_COLUMNS);

        final Frame ret = new DefaultFrame(suppliedTable, storedTable);

        ret.setVisible(true);
        ret.setLayout(new GridLayout(4, 1));
        ret.setPreferredSize(FRAME_DIMENSION);
        ret.setResizable(false);
        ret.addWindowListener(windowListener);

        final JLabel totalSectionLabel = createLabelWithSectionTitle(SECTION_TITLE_OFFSET + TOTAL_SECTION_TITLE);
        final JLabel storedSectionLabel = createLabelWithSectionTitle(SECTION_TITLE_OFFSET + STORED_SECTION_TITLE);
        final JLabel threadNumberSectionLabel = createLabelWithSectionTitle(SECTION_TITLE_OFFSET + WORKERS_COUNT_SECTION_TITLE);
        final JLabel productivitySectionLabel = createLabelWithSectionTitle(SECTION_TITLE_OFFSET + WORK_TIME_SECTION_TITLE);

        final JSlider[] threadsNumberSliders = createThreadNumberSliders(
                config,
                limits,
                bodySuppliersCountListener,
                engineSuppliersCountListener,
                accessorySuppliersCountListener,
                workersCountListener,
                dealersCountListener
        );

        final JSlider[] productivitySliders = createProductivitySliders(
                config,
                limits,
                bodySupplyTimeListener,
                engineSupplyTimeListener,
                accessorySupplyTimeListener,
                carAssemblingTimeListener,
                carRequestTimeListener
        );

        final JPanel totalSection = createTableSection(totalSectionLabel, suppliedTable);
        final JPanel storedSection = createTableSection(storedSectionLabel, storedTable);
        final JPanel productivitySection = createSlidersSection(threadNumberSectionLabel, threadsNumberSliders);
        final JPanel threadsNumberSection = createSlidersSection(productivitySectionLabel, productivitySliders);

        ret.add(totalSection);
        ret.add(storedSection);
        ret.add(productivitySection);
        ret.add(threadsNumberSection);

        loadApplicationIcon(ret);

        return ret;
    }
}
