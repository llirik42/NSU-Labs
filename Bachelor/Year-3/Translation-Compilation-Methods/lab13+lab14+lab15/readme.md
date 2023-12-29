# Lab13

Implement the language of annotated NeMo programs.

# Lab14

Implement the generation of verification conditions for NeMo.

# Lab15

Implement the translator from annotated NeMo programs to the input language of z3.

# Usage

1. Install [z3](https://github.com/Z3Prover/z3)
> You can use `make` with `` -j `nproc` `` to build faster.
2. Install python.
3. Install python-package `z3-solver`

```Bash
pip install z3-solver
```

4. Enjoy verification!

```Bash
flowcpp compiler.flow
```

> You can install `z3-solver` to the host environment and then execute flow9-program as usual. Or you can create python-venv, activate it, install `z3-solver` there and execute flow9-program from terminal with activated venv.

## Using z3-solver in the python-venv

1. Create python-venv

```Bash
python3 -m venv .venv
```

2. Activate it

```Bash
source .venv/bin/activate
```

3. Install `z3-solver` to the venv

```Bash
pip install z3-solver
```

4. Execute flow9-program from terminal with activated venv

```Bash
flowcpp compiler.flow
```

5. Deactivate python-venv (if you need)

```Bash
deactivate
```

> Compiler reads file `./nemo/program.nemo`, creates VC of the program and translates it to the python-script `main.py` (in the same folder of `compiler.flow`), creates process, that executes this python-script, and writes results.

> If compiler cannot create process that executes `main.py`, try replacing `python3` to `python` (in `compiler.flow`).

> `main.py` uses package `z3-solver` so you cannot use compiler without installing core of `z3` or pip-package `z3-solver`.

> `flow9` creates new terminal for every process created by `execSystemProcessOutput`. It means that we cannot activate python-venv and launch process in it from flow9.
