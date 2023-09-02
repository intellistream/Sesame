#!/usr/bin/python3
import math

import matplotlib.ticker as mtick
import os
import click
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

mpl.use('Agg')

show_legend = False
real_world_workloads = ["FCT", "KDD99", "Sensor", "Insects"]
varying_dim = ['20', '40', '60', '80', '100']
compared_algo_name = ['Birch', 'StreamKM++', 'CluStream', 'DenStream', 'DBStream', 'EDMStream', 'DStream', 'SL-KMeans', 'Benne (Efficiency)', 'Benne (Accuracy)']
benne_variant_name = ['Benne (Efficiency)', 'Benne (Accuracy)']
general_colors = [(0.717,0.710,0.627), (0.267, 0.459, 0.478), (0.271, 0.165, 0.239), (0.831, 0.298, 0.235), (0.933, 0.835, 0.718), (0.514, 0.251, 0.149), (0.420, 0.439, 0.361), (0.557,0.714, 0.612), (0.984, 0.518, 0.008), (0.569, 0.835, 0.259)]
benne_colors = [(0.984, 0.518, 0.008), (0.569, 0.835, 0.259)]
benne_acc_colors = ['#c6d900', '#79a701']
benne_eff_colors = ['#e2b400', '#f58a2c']
break_down_colors = ['grey', 'salmon', 'green']
hatches = ['/', '-', 'o', 'x', '+', '.', '*', '|', 'O', '\\']
markers = ['o', 'x', 'd', '*', 's', '^', 'p', '<', 'H', 'P']


def real_world_purity(algo_purity):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.rc('ytick', labelsize=font_size)
    plt.xticks([0.4, 1.4, 2.4, 3.4], real_world_workloads, fontsize=font_size)
    ind = np.arange(4)
    width = 0.25
    for i in range(len(compared_algo_name)):
        plt.bar(ind + width * i / 3 + 0.01, algo_purity[i], width / 3, color=general_colors[i],
                hatch=hatches[i],
                label=compared_algo_name[i],
                edgecolor="k")
    plt.yticks([0.2, 0.4, 0.6, 0.8, 1.0])
    plt.ylabel("Purity", fontsize=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=font_size, ncol=5, frameon=True,
    #                        framealpha=1, edgecolor='black')
    plt.show()
    plt.savefig("jpg/Purity_Real_Existing.jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Purity_Real_Existing.pdf",
                bbox_inches='tight', transparent=True)
    plt.close()
    
def real_world_throughput(algo_throughput):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.rc('ytick', labelsize=font_size)
    plt.xticks([0.4, 1.4, 2.4, 3.4], real_world_workloads, fontsize=font_size)
    ind = np.arange(4)
    width = 0.25
    for i in range(len(compared_algo_name)):
        plt.bar(ind + width * i / 3 + 0.01, algo_throughput[i], width / 3, color=general_colors[i],
                hatch=hatches[i],
                label=compared_algo_name[i],
                edgecolor="k")
    plt.ylabel("Throughput", fontsize=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    ax = plt.gca()
    ax.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=font_size, ncol=5, frameon=True,
    #                        framealpha=1, edgecolor='black')
    plt.show()
    plt.savefig("jpg/Throughput_Real_Existing.jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Throughput_Real_Existing.pdf",
                bbox_inches='tight', transparent=True)
    plt.close()

def eds_cmm_comparison(algo_cmm):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.ylabel('CMM', fontsize=font_size)
    plt.ylim(0, 1)
    ind = np.arange(5)
    plt.xlabel('Phases of EDS Stream', fontsize=font_size)
    plt.xticks([0, 1, 2, 3, 4], [1, 2, 3, 4, 5])
    plt.yticks(size=font_size)
    plt.xticks(size=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    for i in range(len(algo_cmm)):
        plt.plot(ind, algo_cmm[i], '-', markersize=15, color=general_colors[i],
                    marker=markers[i], label=compared_algo_name[i])
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=24, ncol=5, frameon=True,
    #             framealpha=1, edgecolor='black')
    # plt.show()
    plt.savefig("jpg/EDS_CMM.jpg", bbox_inches='tight', transparent=True)
    plt.savefig("pdf/EDS_CMM.pdf", bbox_inches='tight', transparent=True)
    plt.close()
    
def eds_throughput_comparison(algo_throughput):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.ylabel('Throughput', fontsize=font_size)
    ax = plt.gca()
    ax.ticklabel_format(style='sci', scilimits=(-1, 1), axis='y')
    ind = np.arange(5)
    plt.xlabel('Phases of EDS Stream', fontsize=font_size)
    plt.xticks([0, 1, 2, 3, 4], [1, 2, 3, 4, 5])
    plt.yticks(size=font_size)
    plt.xticks(size=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    for i in range(len(algo_throughput)):
        plt.plot(ind, algo_throughput[i], '-', markersize=15, color=general_colors[i],
                    marker=markers[i], label=compared_algo_name[i])
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=24, ncol=5, frameon=True,
    #             framealpha=1, edgecolor='black')
    # plt.show()
    plt.savefig("jpg/EDS_throughput.jpg", bbox_inches='tight', transparent=True)
    plt.savefig("pdf/EDS_throughput.pdf", bbox_inches='tight', transparent=True)
    plt.close()
 
def ods_purity_comparison(algo_purity):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.ylabel('CMM', fontsize=font_size)
    plt.ylim(0, 1)
    ind = np.arange(3)
    plt.xlabel('Phases of ODS Stream', fontsize=font_size)
    plt.xticks([0, 1, 2], [1, 2, 3])
    plt.yticks(size=font_size)
    plt.xticks(size=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    for i in range(len(algo_purity)):
        plt.plot(ind, algo_purity[i], '-', markersize=15, color=general_colors[i],
                    marker=markers[i], label=compared_algo_name[i])
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=24, ncol=5, frameon=True,
    #             framealpha=1, edgecolor='black')
    # plt.show()
    plt.savefig("jpg/ODS_Purity.jpg", bbox_inches='tight', transparent=True)
    plt.savefig("pdf/ODS_Purity.pdf", bbox_inches='tight', transparent=True)
    plt.close()
    
def ods_throughput_comparison(algo_throughput):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.ylabel('Throughput', fontsize=font_size)
    ax = plt.gca()
    ax.ticklabel_format(style='sci', scilimits=(-1, 1), axis='y')
    ind = np.arange(3)
    plt.xlabel('Phases of ODS Stream', fontsize=font_size)
    plt.xticks([0, 1, 2], [1, 2, 3])
    plt.yticks(size=font_size)
    plt.xticks(size=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    for i in range(len(algo_throughput)):
        plt.plot(ind, algo_throughput[i], '-', markersize=15, color=general_colors[i],
                    marker=markers[i], label=compared_algo_name[i])
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=24, ncol=5, frameon=True,
    #             framealpha=1, edgecolor='black')
    # plt.show()
    plt.savefig("jpg/ODS_throughput.jpg", bbox_inches='tight', transparent=True)
    plt.savefig("pdf/ODS_throughput.pdf", bbox_inches='tight', transparent=True)
    plt.close()
 
def dim_purity_comparison(algo_purity):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.rc('ytick', labelsize=font_size)
    plt.xticks([0.15, 1.15, 2.15, 3.15, 4.15], varying_dim, fontsize=font_size)
    ind = np.arange(5)
    width = 0.6
    for i in range(2):
        plt.bar(ind + width * i / 2, algo_purity[i], width / 2, color=benne_colors[i],
                hatch=hatches[i],
                label=benne_variant_name[i],
                edgecolor="k")
    plt.ylim(0.41, 0.44)
    plt.yticks([0.41, 0.42, 0.43, 0.44])
    plt.xlabel('Dimensionality', fontsize=font_size)
    plt.ylabel("Purity", fontsize=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=font_size, ncol=5, frameon=True,
    #                        framealpha=1, edgecolor='black')
    plt.show()
    plt.savefig("jpg/Purity_Dim.jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Purity_Dim.pdf",
                bbox_inches='tight', transparent=True)
    plt.close()
    
def dim_throughput_comparison(algo_throughput):
    plt.figure(figsize=(10, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.rc('ytick', labelsize=font_size)
    plt.xticks([0.15, 1.15, 2.15, 3.15, 4.15], varying_dim, fontsize=font_size)
    ind = np.arange(5)
    width = 0.6
    for i in range(2):
        plt.bar(ind + width * i / 2, algo_throughput[i], width / 2, color=benne_colors[i],
                hatch=hatches[i],
                label=benne_variant_name[i],
                edgecolor="k")
    plt.ylabel("Throughput", fontsize=font_size)
    plt.xlabel('Dimensionality', fontsize=font_size)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    ax = plt.gca()
    ax.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=font_size, ncol=5, frameon=True,
    #                        framealpha=1, edgecolor='black')
    plt.show()
    plt.savefig("jpg/Throughput_Dim.jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Throughput_Dim.pdf",
                bbox_inches='tight', transparent=True)
    plt.close()

def param_outlier_dist(config, purity, throughput, benneAcc=True):
    fig = plt.figure(figsize=(12.5, 7.5))
    font_size = 40
    plt.rcParams['font.size'] = font_size
    ax1 = fig.add_subplot(111)
    # plt.grid(axis='y', linestyle='--', linewidth=0.5)
    if benneAcc:
        plt.xlabel("Outlier Distance Threshold", fontsize=font_size)
        colors = benne_acc_colors
        title = "Acc"
    else:
        plt.xlabel("Outlier Distance Threshold", fontsize=font_size)
        colors = benne_eff_colors
        title = "eff"
    ax1.plot(config, purity, linestyle='-', marker='s',
             markersize=18, color=colors[0], label='Purity')
    ax1.set_ylabel('Purity', fontsize=font_size)
    plt.tick_params(axis='y', size=font_size)
    if min(purity) != max(purity):
        plt.yticks(np.arange(min(purity), max(purity) + (max(purity) - min(purity)) / 4, (max(purity) - min(purity)) / 4),
               size=font_size)
    else:
        plt.yticks(fontsize=font_size)
    plt.xticks(np.arange(20, 120, 20), size=font_size)
    plt.legend(bbox_to_anchor=(0, 1.3), loc='upper left', fontsize=40, frameon=True, framealpha=1,
                   edgecolor='black')

    ax2 = ax1.twinx()  # this is the important function
    ax2.plot(config, throughput, linestyle='-', color=colors[1],
             marker='^', markersize=18, label='Throughput')
    # ax2.set_xlim([0, np.e])
    ax2.set_ylabel('Throughput', fontsize=font_size)
    plt.tick_params(axis='y', size=font_size)
    y_magnitude = math.floor(math.log10(max(throughput)))
    y_magnitude = math.pow(10, y_magnitude)
    interval = np.ceil(round((max(throughput) - min(throughput))/ y_magnitude, 1) * y_magnitude) / 4
    if interval != 0:
        y_tick = np.arange(np.ceil(round(min(throughput) / y_magnitude, 1) * y_magnitude),
                       np.ceil(round(max(throughput) / y_magnitude, 1) * y_magnitude) + interval, interval)
        plt.yticks(y_tick, size=font_size)
    else:
        plt.yticks(fontsize=font_size)
    plt.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
    ax1.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))
    # ax2.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))

    plt.legend(bbox_to_anchor=(1, 1.3), loc='upper right', fontsize=40, frameon=True, framealpha=1,
                   edgecolor='black')
    # plt.show()
    plt.savefig("jpg/Config_Outlier_Distance_" + title +".jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Config_Outlier_Distance_" + title +".pdf",
                bbox_inches='tight', transparent=True)
    plt.close()

def param_queue_size(config, purity, throughput, benneAcc=True):
    fig = plt.figure(figsize=(12.5, 7.5))
    font_size = 40
    plt.rcParams['font.size'] = font_size
    ax1 = fig.add_subplot(111)
    # plt.grid(axis='y', linestyle='--', linewidth=0.5)
    if benneAcc:
        plt.xlabel("Queue Size Threshold", fontsize=font_size)
        colors = benne_acc_colors
        title = "Acc"
    else:
        plt.xlabel("Queue Size Threshold", fontsize=font_size)
        colors = benne_eff_colors
        title = "eff"
    ax1.plot(config, purity, linestyle='-', marker='s',
             markersize=18, color=colors[0], label='Purity')
    ax1.set_ylabel('Purity', fontsize=font_size)
    plt.tick_params(axis='y', size=font_size)
    # if min(purity) != max(purity):
    #     plt.yticks(np.arange(min(purity), max(purity) + (max(purity) - min(purity)) / 4, (max(purity) - min(purity)) / 4),
    #            size=35)
    plt.yticks(fontsize=font_size)
    plt.xticks(np.arange(30000, 120000, 20000), size=font_size)
    # plt.legend(bbox_to_anchor=(0.13, 1.23), loc='upper left', fontsize=24, frameon=True, framealpha=1,
    #                edgecolor='black')

    ax2 = ax1.twinx()  # this is the important function
    ax2.plot(config, throughput, linestyle='-', color=colors[1],
             marker='^', markersize=18, label='Throughput')
    # ax2.set_xlim([0, np.e])
    ax2.set_ylabel('Throughput', fontsize=font_size)
    plt.tick_params(axis='y', size=font_size)
    y_magnitude = math.floor(math.log10(max(throughput)))
    y_magnitude = math.pow(10, y_magnitude)
    interval = np.ceil(round((max(throughput) - min(throughput))/ y_magnitude, 1) * y_magnitude) / 4 + 16600
    if interval != 0:
        y_tick = np.arange(np.ceil(round(min(throughput) / y_magnitude, 1) * y_magnitude) - interval,
                       np.ceil(round(max(throughput) / y_magnitude, 1) * y_magnitude) + interval, interval)
        plt.yticks(y_tick, size=font_size)
    else:
        plt.yticks(fontsize=font_size)
    plt.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
    ax1.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))
    # ax2.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))
    # plt.legend(bbox_to_anchor=(0.9, 1.23), loc='upper right', fontsize=24, frameon=True, framealpha=1,
    #                edgecolor='black')
    # plt.show()
    plt.savefig("jpg/Config_Queue_Size_" + title +".jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Config_Queue_Size_" + title +".pdf",
                bbox_inches='tight', transparent=True)
    plt.close()

def param_variance(config, purity, throughput, benneAcc=True):
    fig = plt.figure(figsize=(12.5, 7.5))
    font_size = 40
    plt.rcParams['font.size'] = font_size
    ax1 = fig.add_subplot(111)
    # plt.grid(axis='y', linestyle='--', linewidth=0.5)
    if benneAcc:
        plt.xlabel("Variance Threshold", fontsize=font_size)
        colors = benne_acc_colors
        title = "Acc"
    else:
        plt.xlabel("Variance Threshold", fontsize=font_size)
        colors = benne_eff_colors
        title = "eff"
    ax1.plot(config, purity, linestyle='-', marker='s',
             markersize=18, color=colors[0], label='Purity')
    ax1.set_ylabel('Purity', fontsize=font_size)
    plt.tick_params(axis='y', size=font_size)
    # if min(purity) != max(purity):
    #     plt.yticks(np.arange(min(purity), max(purity) + (max(purity) - min(purity)) / 4, (max(purity) - min(purity)) / 4),
    #            size=35)
    plt.xticks(np.arange(400, 4400, 800), size=font_size)
    plt.yticks(fontsize=font_size)
    # plt.legend(bbox_to_anchor=(0.13, 1.23), loc='upper left', fontsize=24, frameon=True, framealpha=1,
    #                edgecolor='black')

    ax2 = ax1.twinx()  # this is the important function
    ax2.plot(config, throughput, linestyle='-', color=colors[1],
             marker='^', markersize=18, label='Throughput')
    # ax2.set_xlim([0, np.e])
    ax2.set_ylabel('Throughput', fontsize=font_size)
    plt.tick_params(axis='y', size=font_size)
    y_magnitude = math.floor(math.log10(max(throughput)))
    y_magnitude = math.pow(10, y_magnitude)
    interval = np.ceil(round((max(throughput) - min(throughput))/ y_magnitude, 1) * y_magnitude) / 4
    if interval != 0:
        y_tick = np.arange(np.ceil(round(min(throughput) / y_magnitude, 1) * y_magnitude) - interval,
                       np.ceil(round(max(throughput) / y_magnitude, 1) * y_magnitude) + interval, interval)
        plt.yticks(y_tick, size=font_size)
    else:
        plt.yticks(fontsize=font_size)
    plt.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
    ax1.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))
    # ax2.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))
    # plt.legend(bbox_to_anchor=(0.9, 1.23), loc='upper right', fontsize=24, frameon=True, framealpha=1,
    #                edgecolor='black')
    # plt.show()
    plt.savefig("jpg/Config_Variance_" + title +".jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Config_Variance_" + title +".pdf",
                bbox_inches='tight', transparent=True)
    plt.close()

def breakdown(sum, mig, det, benneAcc=True):
    plt.figure(figsize=(6, 4))
    font_size = 26
    plt.rcParams['font.size'] = font_size
    plt.rc('ytick', labelsize=font_size)
    if benneAcc:
        plt.xticks([0.2, 1.2, 2.2, 3.2], real_world_workloads, fontsize=font_size)
    else:
        plt.xticks([0.05, 1.05, 2.05, 3.05], real_world_workloads, fontsize=font_size)
    ind = np.arange(4)
    width = 0.6
    if benneAcc:
        i = 1
    else:
        i = 0
    plt.bar(ind + width * i / 3 + 0.01, mig[i], width / 3, color=break_down_colors[1],
            hatch=hatches[1],
            label="Migration",
            edgecolor="k")
    plt.bar(ind + width * i / 3 + 0.01, det[i], width / 3, color=break_down_colors[2], bottom=mig[i],
            hatch=hatches[2],
            label="Detection",
            edgecolor="k")
    plt.bar(ind + width * i / 3 + 0.01, sum[i] - mig[i] - det[i], width / 3, color=break_down_colors[0], bottom=det[i],
            hatch=hatches[0],
            label="Clustering",
            edgecolor="k")
    # plt.ylim(0.4, 0.55)
    # plt.yticks([0.4, 0.45, 0.5, 0.55])
    plt.ylabel("Execution Time", fontsize=font_size)
    # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=font_size, ncol=3, frameon=True,
    #                        framealpha=1, edgecolor='black')
    plt.show()
    if benneAcc:
        plt.savefig(f"jpg/Execution_Time_Acc.jpg",
                    bbox_inches='tight', transparent=True)
        plt.savefig("pdf/Execution_Time_Acc.pdf",
                    bbox_inches='tight', transparent=True)
    else:
        plt.savefig(f"jpg/Execution_Time_Eff.jpg",
                    bbox_inches='tight', transparent=True)
        plt.savefig("pdf/Execution_Time_Eff.pdf",
                    bbox_inches='tight', transparent=True)
    plt.close()
@click.command()
@click.option('--real-world', default='raw/real-world.csv', show_default=True)
@click.option('--config-outlier-dist', default='raw/benne-outliers-num.csv', show_default=True)
@click.option('--config-queue-size', default='raw/benne-queue-size.csv', show_default=True)
@click.option('--config-var', default='raw/benne-var.csv', show_default=True)
@click.option('--dim', default='raw/dim-benne.csv', show_default=True)
@click.option('--eds-cmm', default='raw/eds-cmm.csv', show_default=True)
@click.option('--throughput-eds', default='raw/eds-throughput.csv', show_default=True)
@click.option('--purity-ods', default='raw/ods-purity.csv', show_default=True)
@click.option('--throughput-ods', default='raw/ods-throughput.csv', show_default=True)
@click.option('--break-down', default='raw/breakdown.csv', show_default=True)
# below are unnecessary currently
def draw_all_pictures(real_world, config_outlier_dist, config_queue_size, config_var, dim, eds_cmm,
                      throughput_eds, purity_ods, throughput_ods, break_down):
    if real_world != '':
        real_world_data = pd.read_csv(real_world)
        algo_num = 8 + 2
        algo_purity = []
        algo_throughput = []
        for i in range(algo_num):
            algo_purity.append(real_world_data.iloc[i::algo_num, 2])
            algo_throughput.append(real_world_data.iloc[i::algo_num, 3])
        print('---------Purity Comparison of Benne and existing algorithms on real-world workloads---------')
        real_world_purity(algo_purity)
        print('---------Throughput Comparison of Benne and existing algorithms on real-world workloads---------')
        real_world_throughput(algo_throughput)
    if eds_cmm != '':
        cmm = pd.read_csv(eds_cmm)
        algo_num = 8 + 2
        algo_cmm = []
        for i in range(algo_num):
            algo_cmm.append(cmm.iloc[i, 1:6])
        print('---------CMM Comparison of Benne and existing algorithms on EDS workloads---------')
        eds_cmm_comparison(algo_cmm)
    if throughput_eds != '':
        throughput = pd.read_csv(throughput_eds)
        algo_num = 8 + 2
        algo_throughput = []
        for i in range(algo_num):
            algo_throughput.append(throughput.iloc[i, 1:6])   
        print('---------Throughput Comparison of Benne and existing algorithms on EDS workloads---------')
        eds_throughput_comparison(algo_throughput)
    if purity_ods != '':
        purity = pd.read_csv(purity_ods)
        algo_num = 8 + 2
        algo_purity = []
        for i in range(algo_num):
            algo_purity.append(purity.iloc[i, 1:4])
        print('---------Purity Comparison of Benne and existing algorithms on ODS workloads---------')
        ods_purity_comparison(algo_purity)
    if throughput_ods != '':
        throughput = pd.read_csv(throughput_ods)
        algo_num = 8 + 2
        algo_throughput = []
        for i in range(algo_num):
            algo_throughput.append(throughput.iloc[i, 1:4])   
        print('---------Throughput Comparison of Benne and existing algorithms on ODS workloads---------')
        ods_throughput_comparison(algo_throughput)
    if dim != '':
        dimension_data = pd.read_csv(dim)
        algo_num = 8 + 2
        algo_purity = []
        algo_throughput = []
        algo_purity.append(dimension_data.iloc[[0,2,4,6,8], 3].tolist()) 
        algo_purity.append(dimension_data.iloc[[1,3,5,7,9], 3].tolist()) 
        algo_throughput.append(dimension_data.iloc[[0,2,4,6,8], 4].tolist()) 
        algo_throughput.append(dimension_data.iloc[[1,3,5,7,9], 4].tolist()) 
        dim_purity_comparison(algo_purity)
        dim_throughput_comparison(algo_throughput)
    if config_outlier_dist != '':
        config_data = pd.read_csv(config_outlier_dist)
        config = config_data.iloc[0:10, 2].tolist()
        acc_purity = config_data.iloc[0:10, 3].tolist()
        acc_throughput = config_data.iloc[0:10, 4].tolist()
        eff_purity = config_data.iloc[10:, 3].tolist()
        eff_throughput = config_data.iloc[10:, 4].tolist()
        print('---------Parameter Study of outlier distance threshold of Benne on FCT---------')
        param_outlier_dist(config, acc_purity, acc_throughput, benneAcc=True)
        param_outlier_dist(config, eff_purity, eff_throughput, benneAcc=False)
    if config_queue_size != '':
        config_data = pd.read_csv(config_queue_size)
        config = config_data.iloc[0:5, 2].tolist()
        acc_purity = config_data.iloc[0:5, 3].tolist()
        acc_throughput = config_data.iloc[0:5, 4].tolist()
        eff_purity = config_data.iloc[5:, 3].tolist()
        eff_throughput = config_data.iloc[5:, 4].tolist()
        print('---------Parameter Study of queue size threshold of Benne on FCT---------')
        param_queue_size(config, acc_purity, acc_throughput, benneAcc=True)
        param_queue_size(config, eff_purity, eff_throughput, benneAcc=False)
    if config_var != '':
        config_data = pd.read_csv(config_var)
        config = config_data.iloc[0:10, 2].tolist()
        acc_purity = config_data.iloc[0:10, 3].tolist()
        acc_throughput = config_data.iloc[0:10, 4].tolist()
        eff_purity = config_data.iloc[10:, 3].tolist()
        eff_throughput = config_data.iloc[10:, 4].tolist()
        print('---------Parameter Study of variance threshold of Benne on FCT---------')
        param_variance(config, acc_purity, acc_throughput, benneAcc=True)
        param_variance(config, eff_purity, eff_throughput, benneAcc=False)
    if break_down != '':
        config_data = pd.read_csv(break_down)
        sum_time = []
        mig_time = []
        det_time = []
        for i in range(4):
            sum_time.append(config_data.iloc[0][1 + 3 * i])
            mig_time.append(config_data.iloc[0][2 + 3 * i])
            det_time.append(config_data.iloc[0][3 + 3 * i])
        print('---------Execution Time Breakdown on Real-world for Efficiency---------')
        breakdown(sum_time, mig_time, det_time, benneAcc=False)
        sum_time = []
        mig_time = []
        det_time = []
        for i in range(4):
            sum_time.append(config_data.iloc[1][1 + 3 * i])
            mig_time.append(config_data.iloc[1][2 + 3 * i])
            det_time.append(config_data.iloc[1][3 + 3 * i])
        print('---------Execution Time Breakdown on Real-world for Accuracy---------')
        breakdown(sum_time, mig_time, det_time, benneAcc=True)


if __name__ == '__main__':
    folder = os.getcwd()
    if not os.path.exists(folder + '/jpg'):
        os.makedirs(folder + '/jpg')
    if not os.path.exists(folder + '/pdf'):
        os.makedirs(folder + '/pdf')
    draw_all_pictures()
