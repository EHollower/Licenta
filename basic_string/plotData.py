import os
import colorsys
import warnings
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.patheffects as pe
import matplotlib.colors as mcolors 
import matplotlib.font_manager as fm
import matplotlib.ticker as mtick

from collections import defaultdict
from matplotlib.ticker import PercentFormatter, LogLocator, NullLocator, MaxNLocator

color_mapping = {}
color_mapping = {
    # Distinct, non-maxflow-overlapping colors
    'HopcroftKarp0': '#17becf',  # teal
    'HopcroftKarp1': '#e377c2',  # pink
}

style_mapping = {
    'HopcroftKarp0': 'solid',
    'HopcroftKarp1': 'dashed',
}

premium_fp = fm.FontProperties(family = 'DejaVu Sans', weight = 'normal')

def format_alg_code(alg: str) -> str:
    parts = alg.split('-')
    if parts[-1].isdigit():
        code = ''.join(p[0] for p in parts[:-1]) + parts[-1]
    else:
        code = ''.join(p[0] for p in parts)
    if (alg == 'SAP'): code += 'a' 
    if (alg == 'Scaling'): code += 'c'
    return code.lower()

def plotCDF(df, algs, out_dir):
    sub = df[df['Algorithm'].isin(algs)].copy()

    # Compute TLE cutoff (110% of slowest non-TLE)
    runtimes = sub['RuntimeMs'].astype(str)
    non_tle = runtimes.replace('TLE', np.nan).dropna().astype(float)
    max_rt = non_tle.max() * 1.1 if non_tle.size else 1.0

    # Precompute per-alg TLE%
    tle_pct = {}
    for alg in algs:
        tmp = sub[sub['Algorithm'] == alg]
        total = len(tmp)
        tle = (tmp['RuntimeMs'].astype(str) == 'TLE').sum()
        tle_pct[alg] = tle / total * 100 if total else 0


    # Premium figure style
    sns.set_context("talk", font_scale=1.2)
    sns.set_style("white")
    width = max(12, len(algs)*2)
    fig, ax = plt.subplots(figsize=(width, 12))

    for idx, alg in enumerate(algs):
        # include TLEs as points at max_rt, normalize over all instances
        times = (
            sub.loc[sub['Algorithm']==alg, 'RuntimeMs']
               .replace('TLE', max_rt)
               .astype(float)
               .sort_values()
               .values
        )
        total = times.size
        cdf = np.arange(1, total+1) / total
        color = color_mapping.get(alg, sns.color_palette("tab10")[idx])
        style = style_mapping.get(alg, 'solid')

        lable = ['vector <int>', 'basicstring <int>']


        ax.step(
            times, cdf, where='post',
            label=f"{alg[:-1]} ({lable[idx]}))",
            color=color, linewidth=2.5, linestyle=style
        )

        # markers per decade (excluding cutoff if desired)
        decades = np.logspace(
            np.floor(np.log10(times[times>0].min())), 
            np.ceil(np.log10(times.max())),
            int(np.ceil(np.log10(times.max())) - np.floor(np.log10(times[times>0].min())))+1,
            base=10.0
        )
        sel = [np.argmin(np.abs(times - d)) for d in decades if times.min() <= d <= times.max()]
        ax.scatter(
            times[sel], cdf[sel],
            facecolors='white', edgecolors=color,
            s=50, linewidth=1.2, zorder=5
        )

    # log-scale & grid
    ax.set_xscale('log')
    ax.xaxis.set_major_locator(LogLocator(base=10))
    ax.xaxis.set_minor_locator(NullLocator())
    ax.grid(axis='x', which='major', linestyle='--', color='#DDD', alpha=0.2)

    # remove explicit x-limits to avoid non-positive log-scale warning
    ax.set_ylim(0, 1)

    # remove spines, arrowed axes
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.annotate('', xy=(1.02,0), xycoords='axes fraction',
                xytext=(0,0), textcoords='axes fraction',
                arrowprops=dict(arrowstyle='->', lw=1.2, color='black',
                                shrinkA=0, shrinkB=0), clip_on=False)
    ax.annotate('', xy=(0,1.02), xycoords='axes fraction',
                xytext=(0,0), textcoords='axes fraction',
                arrowprops=dict(arrowstyle='->', lw=1.2, color='black',
                                shrinkA=0, shrinkB=0), clip_on=False)

    # TLE cutoff line & label
    # ax.axvline(max_rt, linestyle='--', color='black', linewidth=1.25, alpha=0.7)
    # ax.text(max_rt*1.02, 0.02, 'TLE cutoff',
    #         rotation=90, va='bottom', ha='right',
    #         fontproperties=premium_fp, fontsize=12)

    # labels, %-formatter & legend
    ax.set_xlabel('Timp de execuție (ms)', fontproperties=premium_fp, fontsize=14)
    ax.set_ylabel('Procentajul cumulativ', fontproperties=premium_fp, fontsize=14)
    ax.set_title('Distribuția cumulativă a timpilor de execuție', fontproperties=premium_fp, fontsize=16, pad=15)
    ax.yaxis.set_major_formatter(PercentFormatter(xmax=1))

    leg = ax.legend(title='Algoritmi (str. folosită)', fontsize=12, title_fontsize=14,
                    frameon=True, edgecolor='#444444',
                    loc='center left', bbox_to_anchor=(1.02,0.5))

    fig.subplots_adjust(right=0.80)
    plt.tight_layout()
    graph = os.path.splitext(os.path.basename(df_path))[0]
    alg_codes = "-".join(format_alg_code(a) for a in algs)
    fname = f"{graph}-{alg_codes}"
    fig.savefig(
        os.path.join(out_dir, f"cdf_{fname}.png"),
        dpi=300, bbox_inches='tight'
    )
    plt.close(fig)

# Theme & styling
custom_params = {"axes.spines.right": False, "axes.spines.top": False}
sns.set_theme(style="ticks", rc=custom_params)

df_path = 'basicstring.csv'
out_dir = 'results/'
os.makedirs(out_dir, exist_ok=True)

# Load & filter data
df = pd.read_csv(df_path)


plotCDF(df, ['HopcroftKarp0', 'HopcroftKarp1'], out_dir)
