# Lab11

Implement a NeMo parser.

# Lab12

Implement a virtual NeMo machine, which executes programs, translated from NeMo.

# Usage

> Folders `lingo` and `nemo` must be in root of flow9.

## Compiler

Execute

```Bash
flowcpp compiler.flow
```

from folder with `compiler.flow`.

> Compiler reads file `.../flow9/nemo/program.nemo` and writes a program in the language of the virtual machine to the file `.../flow9/nemo/program.out`.

> Compiler can run tests that are located at the folder `...flow9/nemo/tests`.

## Virtual machine

Execute

```Bash
flowcpp vm.flow
```

from folder with `vm.flow`.

> Virtual machine reads file `.../flow9/nemo/program.out` and executes it.
