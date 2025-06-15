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
from matplotlib.ticker import PercentFormatter, LogLocator, NullLocator, MaxNLocator, LogFormatterMathtext

color_mapping = {
    # Matching algorithms: reuse Dinic colors
    'Dinic': '#55A868',
    'Dinic-Scaling': '#388443',
    # Khun variants: new, distinct colors
    'Khun': '#E24A33',
    'Khun-Arb': "#FA391B",
    'Khun-Rnd': "#FF2200",
    # Hopcroft-Karp variants: new, distinct colors
    'Hopcroft-Karp': "#FF54CC",
    'Hopcroft-Karp-Lin': "#FF00B3",
}

# Mapping for line styles: reuse Dinic, assign for matching variants
style_mapping = {
    'Dinic': 'solid',
    'Dinic-Scaling': 'dashed',
    'Khun': 'solid',
    'Khun-Arb': 'dashed',
    'Khun-Rnd': 'dashdot',
    'Hopcroft-Karp': 'solid',
    'Hopcroft-Karp-Lin': 'dashed',
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
    if (alg == 'Push-Relabel-HG'): code += 'g'
    return code.lower()

def plotAccRate(df, algs, out_dir):
    sns.set_theme(style = 'white', font_scale = 1.1)
    plt.rc('font',   family = 'serif', serif = 'Times New Roman')
    plt.rc('figure', facecolor='white')
    plt.rc('axes',   facecolor='white')

    # compute & sort rates…
    rates = { a: ((df['Algorithm'] == a) & (df['RuntimeMs'].astype(str) != 'TLE')).sum() /  
              max(1,(df['Algorithm'] == a).sum())
              for a in algs 
    }

    sorted_algs, sorted_rates = zip(*sorted(rates.items(), key = lambda x: x[1]))

    # colors from mapping
    colors = [color_mapping[a] for a in sorted_algs]

    # figure
    n = len(algs)
    fig, ax = plt.subplots(figsize=(n*0.8+2, 5))

    # bars with gradient alpha
    bars = ax.bar(np.arange(n), [r * 100 for r in sorted_rates], color = colors, edgecolor = 'white', linewidth = 2)
    
    for bar in bars:
        for yi in np.linspace(0, bar.get_height(), 10):
            ax.fill_between([bar.get_x(), bar.get_x() + bar.get_width()],
                            yi, yi+bar.get_height() / 10,
                            color = bar.get_facecolor(), alpha = 0.02)
    # annotate
    for bar, rate in zip(bars, sorted_rates):
        ax.text(bar.get_x()+bar.get_width() / 2, bar.get_height() + 1,
                f"{rate * 100:.1f}%", ha = 'center', va = 'bottom', fontsize = 12, fontweight = 'bold', color = '#444')

    # styling
    ax.set_title("Rata de Acceptare în Funcție de Algoritm", fontsize = 20, fontweight = 'bold')
    ax.set_ylabel(" Procentajul ratei de acceptare (%)", fontsize = 14)
    ax.set_ylim(0, 110)
    ax.set_xticks(np.arange(n))
    ax.set_xticklabels(sorted_algs, rotation = 30, ha = 'right')
    ax.grid(axis='y', color='#DDD', linestyle='--')

    # save
    fig.tight_layout()
    fname = "-".join(format_alg_code(a) for a in algs)
    fig.savefig(os.path.join(out_dir, f"acceptance_rate_{fname}.png"), dpi=300)
    plt.close(fig)


# Heatmap of average runtime over Nodes x Edges
def plotHeatmap(df, alg, out_dir, node_bins=10, edge_bins=10):
    # Filter and clean data
    sub = df[df['Algorithm'] == alg].copy()
    sub = sub[sub['RuntimeMs'].astype(str) != 'TLE']
    sub['RuntimeMs'] = sub['RuntimeMs'].astype(float)

    # Create bins for nodes and edges
    def custom_bin(series, q, cutoff = 100):
        small_mask = series <= cutoff
        bins = pd.Series(np.nan, index=series.index)
        bins[small_mask] = 0  # Bin 0 for small values

        # qcut on large values
        large_vals = series[~small_mask]
        if not large_vals.empty:
            qcut_bins = pd.qcut(large_vals, q = q - 1, labels = False, duplicates = 'drop')
            bins[~small_mask] = qcut_bins + 1  # Offset by 1 due to bin 0

        return bins.astype(int)

    sub['NodeBin'] = custom_bin(sub['Nodes'], node_bins)
    sub['EdgeBin'] = custom_bin(sub['Edges'], edge_bins)

    # Average runtime per bin
    pivot = sub.pivot_table(index='NodeBin', columns='EdgeBin', values='RuntimeMs', aggfunc='mean')

    def get_bin_labels(series, q, cutoff = 100):
        # first bin is [1, cutoff]
        labels = [f"[1, {cutoff}]"]
        large_vals = series[series > cutoff]

        if not large_vals.empty:
            cat = pd.qcut(large_vals, q = q - 1, duplicates = 'drop')
            # render each pandas.Interval as “[left–right]”
            ranges = [f"[{int(iv.left)}, {int(iv.right - 1)}]" for iv in cat.cat.categories]
            labels += ranges

        return labels

    node_labels = get_bin_labels(sub['Nodes'], node_bins)
    edge_labels = get_bin_labels(sub['Edges'], edge_bins)

    mask_missing = pivot.isna()

    fig, ax = plt.subplots(figsize=(12, 9))

    sns.set_context("talk")
    sns.set_style("white")
    cmap = sns.color_palette("plasma", as_cmap=True)

    with warnings.catch_warnings():
        warnings.simplefilter("ignore", category=RuntimeWarning)
        ax = sns.heatmap(
            pivot,
            cmap = cmap,
            mask = mask_missing,
            linewidths = 1,
            linecolor = 'lightgray',
            annot = True,
            fmt = ".1f",
            annot_kws = {'fontsize':10, 'fontproperties':premium_fp},
            cbar_kws = {'label':' Timp de rulare mediu (ms)'},
            xticklabels = edge_labels,
            yticklabels = node_labels,
            square = True
        )

    cbar = ax.collections[0].colorbar
    cbar.locator = MaxNLocator(nbins = 5, integer = True)
    cbar.update_ticks()

    with warnings.catch_warnings():
        warnings.simplefilter("ignore", category=RuntimeWarning)
        sns.heatmap(
            pivot,
            cmap = mcolors.ListedColormap(['lightgray']),
            mask = ~mask_missing,
            cbar = False,
            linewidths = 1,
            linecolor = 'lightgray',
            xticklabels = edge_labels,
            yticklabels = node_labels,
            square = True,
            ax = ax
        )

    ax.set_xticklabels(edge_labels, rotation = 30, ha = 'right', fontproperties = premium_fp, fontsize = 12)
    ax.set_yticklabels(node_labels, rotation = 0, fontproperties = premium_fp, fontsize = 12)

    alg_color = color_mapping.get(alg, 'black')
    ax.text(0.5, 1.04, alg, transform = ax.transAxes, ha = 'center', va = 'bottom', fontproperties = premium_fp, fontsize = 18, color = alg_color)

    ax.text(0.5, 1.00, 'Timpul de rulare mediu pe clase de noduri și muchii', 
        transform = ax.transAxes,
        ha = 'center', va = 'bottom',
        fontproperties = premium_fp,
        fontsize = 18,
        color = 'black'
    )

    ax.set_xlabel('Muchii', fontproperties = premium_fp, fontsize = 14)
    ax.set_ylabel('Noduri', fontproperties = premium_fp, fontsize = 14)

    plt.tight_layout()
    graph = os.path.splitext(os.path.basename(df_path))[0]
    alg_code = format_alg_code(alg)
    fname = f"{graph}-{alg_code}"
    fig.savefig(
        os.path.join(out_dir, f"heatmap_{fname}.png"),
        dpi=300, bbox_inches='tight'
    )
    plt.close(fig)

def plotTLEHeatmap(df, alg, out_dir, node_bins=10, edge_bins=10):
    # Filter for algorithm
    sub = df[df['Algorithm'] == alg].copy()

    # Mark TLE as 1, others as 0
    sub['IsTLE'] = (sub['RuntimeMs'].astype(str) == 'TLE').astype(int)

    # Binning
    def custom_bin(series, q, cutoff = 100):
        small_mask = series <= cutoff
        bins = pd.Series(np.nan, index = series.index)
        bins[small_mask] = 0
        large_vals = series[~small_mask]
        if not large_vals.empty:
            qcut_bins = pd.qcut(large_vals, q = q - 1, labels = False, duplicates = 'drop')
            bins[~small_mask] = qcut_bins + 1
        return bins.astype(int)

    sub['NodeBin'] = custom_bin(sub['Nodes'], node_bins)
    sub['EdgeBin'] = custom_bin(sub['Edges'], edge_bins)

    # Compute raw counts and % TLE per bin
    counts_mat = sub.groupby(['NodeBin', 'EdgeBin']).size().unstack()
    tle_counts = sub.groupby(['NodeBin', 'EdgeBin'])['IsTLE'].sum().unstack(fill_value=0)
    tle_ratio  = (tle_counts / counts_mat) * 100

    def get_bin_labels(series, q, cutoff = 100):
        # first bin is [1, cutoff]
        labels = [f"[1, {cutoff}]"]
        large_vals = series[series > cutoff]

        if not large_vals.empty:
            cat = pd.qcut(large_vals, q = q - 1, duplicates='drop')
            # render each pandas.Interval as “[left–right]”
            ranges = [f"[{int(iv.left)}, {int(iv.right - 1)}]" for iv in cat.cat.categories]
            labels += ranges

        return labels

    node_labels = get_bin_labels(sub['Nodes'], node_bins)
    edge_labels = get_bin_labels(sub['Edges'], edge_bins)

    # mask out bins that never had any testcases
    mask_missing = counts_mat.isna()

    fig, ax = plt.subplots(figsize=(12, 9))

    sns.set_context("talk")
    sns.set_style("white", {"grid.axis":"y", "grid.linestyle":"--"})
    for line in ax.get_lines(): line.set_linewidth(3)

    # white → red gradient
    cmap = sns.light_palette("red", as_cmap=True)

    with warnings.catch_warnings():
        warnings.simplefilter("ignore", category=RuntimeWarning)
        ax = sns.heatmap(
            tle_ratio,
            cmap=cmap,
            mask=mask_missing,
            linewidths=1,
            linecolor='lightgray',
            annot=False,
            cbar_kws={'label': 'LTD (%)'},
            xticklabels=edge_labels,
            yticklabels=node_labels,
            square=True,
            ax=ax
        )

    cbar = ax.collections[0].colorbar
    cbar.locator = MaxNLocator(nbins=5, integer=True)
    cbar.update_ticks()

    # Fill missing with gray
    with warnings.catch_warnings():
        warnings.simplefilter("ignore", category=RuntimeWarning)
        sns.heatmap(
            tle_ratio,
            cmap=mcolors.ListedColormap(['lightgray']),
            mask=~mask_missing,
            cbar=False,
            linewidths=1,
            linecolor='lightgray',
            xticklabels=edge_labels,
            yticklabels=node_labels,
            square=True,
            ax=ax
        )

    ax.set_xticklabels(edge_labels, rotation=30, ha='right', fontproperties=premium_fp, fontsize=12)
    ax.set_yticklabels(node_labels, rotation=0, fontproperties=premium_fp, fontsize=12)

    alg_color = color_mapping.get(alg, '#D62728')
    ax.text(0.5, 1.04, alg, transform=ax.transAxes, ha='center', va='bottom', fontproperties=premium_fp, fontsize=18, color=alg_color)
    ax.text(0.5, 1.00, 'TLE Rate by Node x Edge Bins',
            transform=ax.transAxes, ha='center', va='bottom',
            fontproperties=premium_fp, fontsize=18, color='black')

    ax.set_xlabel('Edges', fontproperties=premium_fp, fontsize=14)
    ax.set_ylabel('Nodes', fontproperties=premium_fp, fontsize=14)

    # annotate every existing bin (including 0% TLE)
    for i in range(tle_ratio.shape[0]):
        for j in range(tle_ratio.shape[1]):
            # skip bins with no testcases
            if mask_missing.iloc[i, j]:
                continue
            pct = tle_ratio.iloc[i, j]
            # choose text color for readability
            txt_color = 'white' if pct > 50 else 'black'
            ax.text(
                j + 0.5, i + 0.5,
                f"{pct:.1f}%",
                ha='center', va='center',
                color=txt_color,
                fontproperties=premium_fp,
                fontsize=10
            )

    plt.tight_layout()
    graph = os.path.splitext(os.path.basename(df_path))[0]
    alg_code = format_alg_code(alg)
    fname = f"{graph}-{alg_code}"
    fig.savefig(
        os.path.join(out_dir, f"tle_heatmap_{fname}.png"),
        dpi=300, bbox_inches='tight'
    )
    plt.close(fig)

def plotCDF(df, algs, out_dir):
    sub = df[df['Algorithm'].isin(algs)].copy()

    # Compute TLE cutoff (110% of slowest non-TLE)
    runtimes = sub['RuntimeMs'].astype(str)
    non_tle = runtimes.replace('TLE', np.nan).dropna().astype(float)
    max_rt = 1e4 if non_tle.size else 1.0

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

        ax.step(
            times, cdf, where='post',
            label=f"{alg} ({tle_pct[alg]:.0f}%)",
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
    ax.axvline(max_rt, linestyle='--', color='black', linewidth=1.25, alpha=0.7)
    ax.text(max_rt*1.02, 0.02, 'Limita de timp (ms)',
            rotation=90, va='bottom', ha='right',
            fontproperties=premium_fp, fontsize=12)

    # labels, %-formatter & legend
    ax.set_xlabel('Timpul de execuție (ms)', fontproperties=premium_fp, fontsize=14)
    ax.set_ylabel('Procentajul cumulativ', fontproperties=premium_fp, fontsize=14)
    ax.set_title('Distribuția cumulativă a timpilor de execuție', fontproperties=premium_fp, fontsize=16, pad=15)
    ax.yaxis.set_major_formatter(PercentFormatter(xmax=1))

    leg = ax.legend(title='Algoritmi (LTE %)', fontsize=12, title_fontsize=14,
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

def print_geometric_mean_ratio(df, alg1, alg2):
    """Compute and print the geometric mean of runtime ratios (alg2/alg1) across common testcases."""
    # filter out TLE and convert to float
    sub1 = df[df['Algorithm'] == alg1].copy()
    sub2 = df[df['Algorithm'] == alg2].copy()
    sub1 = sub1[sub1['RuntimeMs'].astype(str) != 'TLE']
    sub2 = sub2[sub2['RuntimeMs'].astype(str) != 'TLE']
    sub1['RuntimeMs'] = sub1['RuntimeMs'].astype(float)
    sub2['RuntimeMs'] = sub2['RuntimeMs'].astype(float)
    # merge on Testcase
    merged = pd.merge(
        sub1[['Testcase','RuntimeMs']],
        sub2[['Testcase','RuntimeMs']],
        on='Testcase', suffixes=(f'_{alg1}', f'_{alg2}')
    )
    # compute ratios
    # replace zero denominators with minimal constant to avoid division by zero
    eps = 1e-3
    denom = merged[f'RuntimeMs_{alg1}'].replace(0, eps)
    ratios = merged[f'RuntimeMs_{alg2}'] / denom
    # filter to positive, finite ratios to avoid log errors
    valid = ratios[np.isfinite(ratios) & (ratios > eps)]
    n = len(valid)
    if n == 0:
        print(f"No valid positive ratios for {alg2}/{alg1}")
        return float('nan')
    # geometric mean
    gm = np.exp(np.mean(np.log(valid)))
    print(f"Geometric mean of {alg2}/{alg1} over {n} testcases: {gm:.4f}")
    return gm

# Theme & styling
custom_params = {"axes.spines.right": False, "axes.spines.top": False}
sns.set_theme(style="ticks", rc=custom_params)

# Paths
df_path = 'maxmatching.csv'
out_dir = 'results'
os.makedirs(out_dir, exist_ok=True)

# Load & filter data
df = pd.read_csv(df_path)

# plotAccRate(df, ['Dinic', 'Dinic-Scaling', 'Khun', 'Khun-Arb', 'Khun-Rnd', 'Hopcroft-Karp', 'Hopcroft-Karp-Lin'], out_dir)

# plotHeatmap(df, 'Khun', out_dir + '/heatmaps')
# plotHeatmap(df, 'Khun-Arb', out_dir + '/heatmaps')
# plotHeatmap(df, 'Khun-Rnd', out_dir + '/heatmaps')

# plotTLEHeatmap(df, 'Khun', out_dir + '/heatmaps')
# plotTLEHeatmap(df, 'Khun-Arb', out_dir + '/heatmaps')
# plotTLEHeatmap(df, 'Khun-Rnd', out_dir + '/heatmaps')

# plotCDF(df, ['Khun-Rnd', 'Khun-Arb', 'Khun'], out_dir + '/comparisons')

# plotHeatmap(df, 'Dinic', out_dir + '/heatmaps')
# plotHeatmap(df, 'Dinic-Scaling', out_dir + '/heatmaps')

# plotTLEHeatmap(df, 'Dinic', out_dir + '/heatmaps')
# plotTLEHeatmap(df, 'Dinic-Scaling', out_dir + '/heatmaps')

# plotCDF(df, ['Dinic', 'Dinic-Scaling'], out_dir + '/comparisons')
# plotCDF(df, ['Khun-Rnd', 'Dinic'], out_dir + '/comparisons')

# plotHeatmap(df, 'Hopcroft-Karp', out_dir + '/heatmaps')
# plotHeatmap(df, 'Hopcroft-Karp-Lin', out_dir + '/heatmaps')

# plotTLEHeatmap(df, 'Hopcroft-Karp', out_dir + '/heatmaps')
# plotTLEHeatmap(df, 'Hopcroft-Karp-Lin', out_dir + '/heatmaps')

# plotCDF(df, ['Hopcroft-Karp-Lin', 'Hopcroft-Karp'], out_dir + '/comparisons')
# plotCDF(df, ['Hopcroft-Karp-Lin', 'Khun-Rnd', 'Dinic'], out_dir + '/comparisons')

