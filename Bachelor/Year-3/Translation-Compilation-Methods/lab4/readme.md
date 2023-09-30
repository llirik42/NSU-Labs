# Lab4

Given an array of integers $[n_1, \dots, n_k]$ and a number $m$, find all pairs of indexes $(i, j)$ such that $n_i + n_j == m$. Signature of a function: `inds(a : [int], m : int) -> [Pair<int, int>]`. *Complication*: make this function of complexity $O(nlog(n))$, not $O(n^2)$. **Use functions**: `foldi`, `makeTree`, `setTree`, `lookupTree`.
