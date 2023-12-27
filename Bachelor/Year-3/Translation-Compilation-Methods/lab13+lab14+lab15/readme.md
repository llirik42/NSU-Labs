# Lab13

Implement the language of annotated NeMo programs.

# Lab14

Implement the generation of verification conditions for NeMo.

# Lab15

Implement the translator from annotated NeMo programs to the input language of z3.

# Usage

1. Move folders `lingo` and `nemo` to the root of flow9.
2. Install [z3](https://github.com/Z3Prover/z3)
> You can use `make` with ``make -j `nproc` `` to build faster.
3. Install python.
4. Install pip-package `z3-solver`. 

```Bash
pip install z3-solver
```

5. Enjoy verification!

```Bash
flowcpp compiler.flow
```
> Compiler reads file `.../flow9/nemo/program.nemo`, creates VC of the program and translates it to the python-script `main.py` (in the same folder of `compiler.flow`), creates process that executes this python-script (using packages `z3-solver`). Then compiler writes results.
> If compiler cannot create process that executes `main.py`, try replacing `python` to `python3` (in `compiler.flow`).
