"""
Reformat LOJ “#101. 最大流” test cases (1-indexed) into 0-indexed format.
Download from: https://loj.ac/p/101/files (login required).

Original input:
 V E s t
 u_i v_i cap

Reformatted:
 V E s-1 t-1
 u_i v_i cap (0-indexed)
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '8_loj_101.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    V, E, s, t = map(int, lines[0].split())
    header = f"{V} {E} {s-1} {t-1}"
    edges = [f"{int(u)-1} {int(v)-1} {cap}" \
             for u, v, cap in (line.split() for line in lines[1:])]
    out_file = OUT_DIR / f"loj101-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir(): f.unlink()
TMP_DIR.rmdir()