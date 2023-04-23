package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.controller.Controller;
import ru.nsu.kondrenko.gui.controller.Properties;
import ru.nsu.kondrenko.model.game.Context;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.PieceContainer;

import javax.swing.*;
import java.awt.*;

public class SingleplayerFrame extends FixedSizeFrame {
    private static final Dimension FRAME_DIMENSION = new Dimension(600, 600);
    private static final Dimension BUTTON_DIMENSION = new Dimension(400, 150);
    private static final Font BUTTON_FONT = new Font("Ubuntu", Font.PLAIN, 36);

    public SingleplayerFrame(Controller controller) {
        super(FRAME_DIMENSION);

        setLayout(new GridBagLayout());
        GridBagConstraints gridBagConstraints = new GridBagConstraints();
        gridBagConstraints.gridwidth = GridBagConstraints.REMAINDER;
        gridBagConstraints.fill = GridBagConstraints.HORIZONTAL;

        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);

        final JRadioButton whiteColorButton = new JRadioButton("White");
        final JRadioButton blackColorButton = new JRadioButton("Black");
        final JRadioButton randomColorButton = new JRadioButton("Random");
        final JTextField playerNameField = new JTextField();
        final JButton submitButton = new JButton("Submit");

        whiteColorButton.setActionCommand(Properties.PICKED_WHITE_COLOR);
        blackColorButton.setActionCommand(Properties.PICKED_BLACK_COLOR);
        randomColorButton.setActionCommand(Properties.PICKED_RANDOM_COLOR);
        playerNameField.setActionCommand(Properties.SINGLE_PLAYER_NAME_INPUT);
        submitButton.setActionCommand(Properties.SUBMIT_SINGLEPLAYER_ACTION_COMMAND);

        whiteColorButton.setPreferredSize(BUTTON_DIMENSION);
        blackColorButton.setPreferredSize(BUTTON_DIMENSION);
        randomColorButton.setPreferredSize(BUTTON_DIMENSION);
        playerNameField.setPreferredSize(BUTTON_DIMENSION);
        submitButton.setPreferredSize(BUTTON_DIMENSION);

        whiteColorButton.addItemListener(controller);
        blackColorButton.addItemListener(controller);
        randomColorButton.addItemListener(controller);
        playerNameField.getDocument().putProperty(Properties.DOCUMENT_NAME_PROPERTY, Properties.NAME_OF_DOCUMENT_WITH_SINGLE_PLAYER_NAME);
        playerNameField.getDocument().addDocumentListener(controller);
        submitButton.addActionListener(controller);

        whiteColorButton.setFont(BUTTON_FONT);
        blackColorButton.setFont(BUTTON_FONT);
        randomColorButton.setFont(BUTTON_FONT);
        playerNameField.setFont(BUTTON_FONT);
        submitButton.setFont(BUTTON_FONT);

        final JMenu menu = new JMenu("back");
        menu.setActionCommand(Properties.BACK);
        menu.addMenuListener(controller);

        final JMenuBar menuBar = new JMenuBar();
        menuBar.add(menu);
        setJMenuBar(menuBar);

        final ButtonGroup group = new ButtonGroup();
        group.add(whiteColorButton);
        group.add(blackColorButton);
        group.add(randomColorButton);

        add(whiteColorButton, gridBagConstraints);
        add(blackColorButton, gridBagConstraints);
        add(randomColorButton, gridBagConstraints);
        add(playerNameField, gridBagConstraints);
        add(submitButton, gridBagConstraints);

        SwingUtilities.invokeLater(this::pack);
    }

    @Override
    public void notifyBoardChange(PieceContainer pieceContainer, Context context) {

    }

    @Override
    public void notifyGameStateChange(GameState state, Context context) {

    }
}
