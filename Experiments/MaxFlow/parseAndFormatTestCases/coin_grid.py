"""
Reformat CSES “Coin Grid” test cases into bipartite flow graph input.
Download from: https://cses.fi/problemset/tests/1709/ (login required)
Each .in contains an nxn grid of '.' and 'o'.
Nodes: numbered 0=src, 1..n=rows, n+1..2n=cols, 2n+1=sink.
Edges:
src→row_i cap=1
row_i→col_j cap=1 if grid[i][j]=='o'
col_j→sink cap=1
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '5_coin_grid.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    grid = in_path.read_text().splitlines()
    n = len(grid)
    src, sink = 0, 2*n+1
    N = sink + 1
    edges = []
    edges += [f"{src} {i+1} 1" for i in range(n)]
    for i, row in enumerate(grid):
        for j, c in enumerate(row):
            if c == 'o':
                edges.append(f"{i+1} {n+j+1} 1000000")
    # cols→sink
    edges += [f"{n+i+1} {sink} 1" for i in range(n)]
    M = len(edges)
    header = f"{N} {M} {src} {sink}"
    out_file = OUT_DIR / f"coingrid-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir():
    f.unlink()
TMP_DIR.rmdir()