package ru.nsu.kondrenko.gui.view;

import ru.nsu.kondrenko.gui.controller.Properties;
import ru.nsu.kondrenko.model.game.Context;
import ru.nsu.kondrenko.model.game.GameState;
import ru.nsu.kondrenko.model.game.PieceContainer;
import ru.nsu.kondrenko.model.statistics.PlayerScoreRecord;
import ru.nsu.kondrenko.model.statistics.StatisticsStorage;

import javax.swing.*;
import javax.swing.event.MenuListener;
import java.awt.*;

public class StatisticsFrame extends FixedSizeFrame {
    private static final Font TABLE_FONT = new Font("Courier", Font.PLAIN, 30);
    private static final Dimension FRAME_DIMENSION = new Dimension(600, 600);
    private static final int ROW_HEIGHT = 30;

    public StatisticsFrame(final StatisticsStorage statisticsStorage, final MenuListener menuListener) {
        super(FRAME_DIMENSION);

        setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        final JTable table = getTable(statisticsStorage);

        final JScrollPane scrollPane = new JScrollPane(table);

        final JMenu menu = new JMenu("back");
        menu.setActionCommand(Properties.BACK);
        menu.addMenuListener(menuListener);

        final JMenuBar menuBar = new JMenuBar();
        menuBar.add(menu);
        setJMenuBar(menuBar);

        add(scrollPane);

        SwingUtilities.invokeLater(this::pack);
    }

    private JTable getTable(StatisticsStorage statisticsStorage) {
        final String[] columnNames = {"Player", "Score"};
        final Object[][] data = new Object[statisticsStorage.getCountOfRecords()][];

        for (int i = 0; i < statisticsStorage.getCountOfRecords(); i++) {
            final PlayerScoreRecord record = statisticsStorage.getRecord(i);
            data[i] = new Object[]{record.playerName(), record.score()};
        }

        final JTable table = new JTable(data, columnNames) {
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };

        table.getTableHeader().setFont(TABLE_FONT);
        table.setFont(TABLE_FONT);
        table.setFillsViewportHeight(true);
        table.setRowHeight(ROW_HEIGHT);

        return table;
    }

    @Override
    public void notifyBoardChange(PieceContainer pieceContainer, Context context) {

    }

    @Override
    public void notifyGameStateChange(GameState state, Context context) {

    }
}
