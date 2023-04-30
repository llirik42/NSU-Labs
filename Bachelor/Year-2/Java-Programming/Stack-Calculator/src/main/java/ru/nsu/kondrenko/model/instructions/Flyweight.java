package ru.nsu.kondrenko.model.instructions;

import ru.nsu.kondrenko.model.context.Context;
import ru.nsu.kondrenko.model.exceptions.DivisionByZeroException;
import ru.nsu.kondrenko.model.exceptions.FlyweightException;

import java.util.HashMap;
import java.util.Map;

public class Flyweight implements Instruction {
    @FunctionalInterface
    public interface BinaryOperation {
        void execute(double operand1, double operand2, Context context);
    }

    private final Map<FlyweightOperation, BinaryOperation> operations = new HashMap<>();

    {
        operations.put(FlyweightOperation.ADD, (o1, o2, ctx) -> ctx.pushValue(o1 + o2));
        operations.put(FlyweightOperation.SUBTRACT, (o1, o2, ctx) -> ctx.pushValue(o1 - o2));
        operations.put(FlyweightOperation.MULTIPLY, (o1, o2, ctx) -> ctx.pushValue(o1 * o2));
        operations.put(FlyweightOperation.DIVIDE, (o1, o2, ctx) -> {
            if (o2 == 0) {
                throw new DivisionByZeroException();
            }

            ctx.pushValue(o1 / o2);
        });

    }

    private FlyweightOperation operationType;

    public void setOperationType(FlyweightOperation operationType) {
        this.operationType = operationType;
    }

    @Override
    public void execute(Context context, String[] operands) {
        if (operationType == null) {
            throw new FlyweightException();
        }

        double operand1 = context.pop();
        double operand2 = context.pop();
        operations.get(operationType).execute(operand1, operand2, context);
    }

    @Override
    public int getNeededElementsCountOnStack() {
        if (operationType == null) {
            throw new FlyweightException();
        }

        return 2;
    }

    @Override
    public int getNeededOperandsCount() {
        if (operationType == null) {
            throw new FlyweightException();
        }

        return 0;
    }
}
