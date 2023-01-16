#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import click
import os
import matplotlib.ticker as mtick
from typing import List

show_legend = True
real_world_workloads = ["FCT", "KDD99", "Sensor", "Insects"]
synthetic_workloads = ['EDS', 'ODS']
varying_arr_rate = ['2k/s', '20k/s', '50k/s', '100k/s']
design_aspects = ['WindowModel', 'OutlierDetection', 'DataStructure', 'OfflineRefinement']
general_algo_name = ['Birch', 'StreamKM++', 'CluStream', 'DenStream', 'DBStream', 'EDMStream', 'DStream', 'SL-KMeans', 'Benne (Accuracy)', 'Benne (Efficiency)']
generic_algo_name = ['G1', 'G2', 'G3', 'G4', 'G5', 'G6', 'G7', 'G8', 'G9', 'G10']
general_colors = ['orange', 'gold', 'yellowgreen', 'grey', 'seagreen', 'cornflowerblue', 'crimson', 'blue']
generic_colors = ['salmon', 'peru', 'lawngreen', 'turquoise', 'mediumslateblue', 'cornflowerblue', 'violet', 'hotpink',
                  'silver', 'darkorange', 'slategrey', 'chocolate', 'lime', 'royalblue', 'plum', 'linen', 'wheat',
                  'powderblue', "darkorange", "bisque", "cyan", "lightcyan"]
etb_colors = ['orange', 'cornflowerblue', 'green', 'maroon']
hatches = ['/', '-', 'o', 'x', '+', '.', '*', '|', 'O', '\\']
markers = ['o', 'x', 'd', '*', 's', '^', 'p', '<', 'H', 'P']

# 2,3,5,6
def purity_and_throughput(legend_name, data, colors, purity, general,dimension):
    if general:
        font = 36
    else:
        font = 36
    plt.rcParams['font.size'] = font
    plt.rc('ytick', labelsize=font)
    plt.figure(figsize=(10, 4))
    if dimension:
        plt.xticks([0.3, 1.3], ["High", "Low"], fontsize=14)
        ind = np.arange(2)
        plt.xlabel('Dimension of workload', fontsize=18)
    else:
        plt.xticks([0.3, 1.3, 2.3, 3.3], real_world_workloads, fontsize=font)
        ind = np.arange(4)
    width = 0.25
    name = legend_name
    # if not purity and not dimension:  # throughput
    #     data = np.log(data)
    for i in range(len(legend_name)):
        if len(data) == 10:
            w = width / 3
            start = ind + width * i / 3 + 0.01
        if len(data) == 6:
            w = width / 2
            start = ind + width * i / 2 -0.01
        elif len(data) == 2:
            w = width
            start = ind + 0.15 + width * i
        elif len(data) == 3:
            w = width
            start = ind + 0.05 + width* i
        elif len(data) == 4:
            w = width / 2
            start = ind + width * i / 2 + 0.01
        elif len(data) == 5:
            w = width/1.5
            start = ind + width * i / 1.5 - 0.025
            name = "Outlier_Detection"
        plt.bar(start, data[i], w, color=generic_colors[colors[i]],
                hatch=hatches[i],
                label=legend_name[i],
                edgecolor="k")
    if len(legend_name[0]) == 24:
        name = "offline"
    if purity:
        plt.yticks([0.2, 0.4, 0.6, 0.8, 1.0])
        plt.ylabel("Purity", fontsize=font)
        plt.grid(axis='y', linestyle='--', linewidth=0.5)
        if show_legend:
            if len(data) == 4:
                plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=26, ncol=2, frameon=True,
                           framealpha=1, edgecolor='black')
            else:
                plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=26, ncol=6, frameon=True,
                           framealpha=1, edgecolor='black')
        # plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=14, ncol=10)
        # plt.show()
        if general:
            name = ""
            plt.savefig("jpg/Purity_Real_General " + str(name) + ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Purity_Real_General " + str(name) + ".pdf", bbox_inches='tight', transparent=True)
        else:
            plt.savefig("jpg/Purity_Real_Generic " + str(name) + ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Purity_Real_Generic " + str(name) + ".pdf", bbox_inches='tight', transparent=True)
    elif dimension:
        plt.yticks([5, 20, 40,60, 80])
        plt.ylabel("Proportion (%)", fontsize=18)
        plt.grid(axis='y', linestyle='--', linewidth=0.5)
        # plt.legend(bbox_to_anchor=(0.5, 1.42), loc=9, borderaxespad=0, fontsize=14, ncol=3)
        # plt.show()
        plt.savefig("jpg/Dim_FCT_INSECTS " + str(name) + ".jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/Dim_FCT_INSECTS " + str(name) + ".pdf", bbox_inches='tight', transparent=True)
    else:
        plt.ylabel("Throughput", fontsize=font)
        plt.grid(axis='y', linestyle='--', linewidth=0.5)
        ax = plt.gca()
        ax.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
        if show_legend:
            if len(data) == 4:
                plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=26, ncol=2, frameon=True,
                           framealpha=1, edgecolor='black')
            else:
                plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=26, ncol=6, frameon=True,
                           framealpha=1, edgecolor='black')
        # plt.show()
        if general:
            name = ""
            plt.savefig("jpg/Throughput_Real_General " + str(name) + ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Throughput_Real_General " + str(name) + ".pdf", bbox_inches='tight', transparent=True)
        else:
            plt.savefig("jpg/Throughput_Real_Generic " + str(name) + ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Throughput_Real_Generic " + str(name) + ".pdf", bbox_inches='tight', transparent=True)
    plt.close()


def draw_line(legend_name, data, colors, ods, throughput, purity, window, id):
    plt.figure(figsize=(10, 4))
    plt.rcParams['font.size'] = 36
    if throughput:
        # data = np.log(data)
        file_name = "Throughput_"
        plt.ylabel('Throughput', fontsize=36)
        ax = plt.gca()
        ax.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
        legend_position = (0.45, 1.26)
    elif purity:
        file_name = "Purity_"
        plt.ylabel('Purity', fontsize=36)
        plt.ylim(0.6, 0.8)
        legend_position = (0.45, 1.25)
    elif window:
        file_name = "Window_"
        plt.ylabel('Cluster Number', fontsize=36)
        data = np.log(data)
        legend_position = (0.45, 1.26)
    else:
        file_name = "CMM_"
        plt.ylabel('CMM', fontsize=36)
        plt.ylim(0, 1)
        legend_position = (0.45, 1.45)
    if ods:
        ind = np.arange(3)
        file_name = file_name + "ODS_"
        plt.xlabel('Phases of ODS Stream', fontsize=36)
        plt.xticks([0, 1, 2], [1, 2, 3])
    elif window:
        if id == 0:
            file_name = file_name + "FCT_"
        elif id == 1:
            file_name = file_name + "EDS_"
        elif id == 2:
            file_name = file_name + "EDS(Outlier)_"
        ind = np.array([0,2,4,6,8,10,12,14,16,18])
        plt.xlabel('Length of the stream (x'+ '$ \mathit{10}^{4}$'+ ')', fontsize=36)
        plt.xticks(ind, ind + 2)
    else:
        ind = np.arange(5)
        file_name = file_name + "EDS_"
        plt.xlabel('Phases of EDS Stream', fontsize=36)
        plt.xticks([0, 1, 2, 3, 4], [1, 2, 3, 4, 5])
    plt.yticks(size=36)
    plt.xticks(size=36)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    if ods:
        for i in range(len(legend_name)):
            plt.plot(ind, data[i], '-', markersize=15, color=generic_colors[colors[i]],
                     marker=markers[i], label=legend_name[i])
        # plt.legend(bbox_to_anchor=legend_position, loc=9, borderaxespad=0, fontsize=14, ncol=10)
        # plt.show()
        plt.savefig("jpg/" + file_name + "Generic " + str(legend_name) + ".jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/" + file_name + "Generic " + str(legend_name) + ".pdf", bbox_inches='tight', transparent=True)
    else:
        for i in range(len(legend_name)):
            plt.plot(ind, data[i], '-', markersize=15, color=generic_colors[colors[i]],
                     marker=markers[i], label=legend_name[i])
        if show_legend:
            plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=24, ncol=6, frameon=True, framealpha=1, edgecolor='black')
        # plt.show()
        if len(data) == 5:
            legend_name = '_Timer'
        plt.savefig("jpg/" + file_name + "Generic " + str(legend_name) + ".jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/" + file_name + "Generic " + str(legend_name) + ".pdf", bbox_inches='tight', transparent=True)
    plt.close()


def configer(config, purity, throughput, measurement):
    fig = plt.figure(figsize=(10,6))
    plt.rcParams['font.size'] = 36
    ax1 = fig.add_subplot(111)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    ax1.plot(config, purity, linestyle='-', marker='s', markersize=18, color='b', label='Purity')
    ax1.set_ylabel('Purity', fontsize=36)
    plt.xlabel(measurement, fontsize=36)
    plt.tick_params(axis='y', size=35)
    plt.yticks(size=35)
    plt.xticks(size=35)
    if show_legend:
        plt.legend(bbox_to_anchor=(0.13, 1.23), loc='upper left', fontsize=24, frameon=True, framealpha=1, edgecolor='black')

    ax2 = ax1.twinx()  # this is the important function
    ax2.plot(config, throughput, linestyle='-', color='r', marker='^', markersize=18, label='Throughput')
    # ax2.set_xlim([0, np.e])
    ax2.set_ylabel('Throughput', fontsize=36)
    plt.tick_params(axis='y', size=36)
    plt.yticks(size=36)
    plt.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
    ax1.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))

    # 图例
    if show_legend:
        plt.legend(bbox_to_anchor=(0.9, 1.23), loc='upper right', fontsize=24, frameon=True, framealpha=1, edgecolor='black')
    # plt.show()
    plt.savefig("jpg/Config_" + measurement + ".jpg", bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Config_" + measurement + ".pdf", bbox_inches='tight', transparent=True)
    plt.close()


# def execution_time_breakdown(reader, general, workload):
#     plt.figure(figsize=(5, 3))
#     plt.rcParams['font.size'] = '11'
#     plt.ylabel('Percentage(%)', fontsize=11)
#     plt.rc('ytick', labelsize=11)
#     width = 0.35
#     Bottom = reader[design_aspects[0]].values
#     Center1 = reader[design_aspects[1]].values
#     Center2 = reader[design_aspects[2]].values
#     Top = reader[design_aspects[3]].values
#     Sum = Bottom + Center1 + Center2 + Top
#     Bottom = (Bottom / Sum).tolist()
#     Center1 = (Center1 / Sum).tolist()
#     Center2 = (Center2 / Sum).tolist()
#     Top = (Top / Sum).tolist()
#     AccumulateCenter1 = []
#     AccumulateCenter2 = []
#     AccumulateCenter3 = []
#     for i in range(0, len(Bottom)):
#         sum = Bottom[i] + Center1[i]
#         AccumulateCenter1.append(sum)
#     for i in range(0, len(AccumulateCenter1)):
#         sum = AccumulateCenter1[i] + Center2[i]
#         AccumulateCenter2.append(sum)
#     for i in range(0, len(AccumulateCenter2)):
#         sum = AccumulateCenter2[i] + Top[i]
#         AccumulateCenter3.append(sum)
#     if general:
#         ind = np.arange(len(general_algo_name))
#         plt.xticks(ind, general_algo_name, rotation=25, size=11)
#         plt.bar(ind, Bottom[0:8], width, label="Window Model", color=etb_colors[0], hatch=hatches[3])
#         plt.bar(ind, Center1[0:8], width, label="Outlier Detection", bottom=Bottom[0:8],
#                 color=etb_colors[1],
#                 hatch=hatches[1])
#         plt.bar(ind, Center2[0:8], width, label="Data Structure", bottom=AccumulateCenter1[0:8],
#                 color=etb_colors[2],
#                 hatch=hatches[2])
#         plt.bar(ind, Top[0:8], width, label="Offline Refinement", bottom=AccumulateCenter2[0:8],
#                 color=etb_colors[3],
#                 hatch=hatches[0])
#         plt.legend(loc=1, bbox_to_anchor=(1.02, 1.28), ncol=2, prop={'size': 11})
#         # plt.show()
#         plt.savefig("jpg/ETB_" + workload + "_General.jpg", bbox_inches='tight', transparent=True)
#         plt.savefig("pdf/ETB_" + workload + "_General.pdf", bbox_inches='tight', transparent=True)
#     else:
#         ind = np.arange(len(generic_algo_name))
#         plt.xticks(ind, generic_algo_name, rotation=25, size=11)
#         plt.xticks(ind, generic_algo_name, rotation=25, size=11)
#         plt.bar(ind, Bottom[8:18], width, label=design_aspects[0], color=etb_colors[0], hatch=hatches[3])
#         plt.bar(ind, Center1[8:18], width, label=design_aspects[1], bottom=Bottom[8:18],
#                 color=etb_colors[1],
#                 hatch=hatches[1])
#         plt.bar(ind, Center2[8:18], width, label=design_aspects[2], bottom=AccumulateCenter1[8:18],
#                 color=etb_colors[2],
#                 hatch=hatches[2])
#         plt.bar(ind, Top[8:18], width, label=design_aspects[3], bottom=AccumulateCenter2[8:18],
#                 color=etb_colors[3],
#                 hatch=hatches[0])
#         plt.legend(loc=1, bbox_to_anchor=(1.02, 1.28), ncol=2, prop={'size': 11})
#         # plt.show()
#         plt.savefig("jpg/ETB_" + workload + "_Generic.jpg", bbox_inches='tight', transparent=True)
#         plt.savefig("pdf/ETB_" + workload + "_Generic.pdf", bbox_inches='tight', transparent=True)
#     plt.close()
# def progressiveness(reader, general, workload):
#     plt.figure(figsize=(8, 4))
#     ind = np.arange(0, 6)
#     plt.rcParams['font.size'] = 18
#     plt.xlabel('Elapsed Time (usec)', fontsize=18)
#     plt.ylabel('Cumulative Percent', fontsize=18)
#     plt.yticks(ind, ('0', '20%', '40%', '60%', '80%', ' 100%'), size=18)
#     plt.xticks(size=18)
#     plt.grid(axis='y', linestyle='--', linewidth=0.5)
#     if general:
#         for i in range(len(general_algo_name)):
#             elapse_time = reader.iloc[i].to_list()[1:]
#             elapse_time.insert(0, 0)
#             plt.plot(elapse_time, ind, '-', markersize=10, color=general_colors[i],
#                      marker=markers[i], label=general_algo_name[i])
#         plt.legend(bbox_to_anchor=(0.44, 1.2), loc=9, borderaxespad=0, fontsize=12, ncol=4)
#         # plt.show()
#         plt.savefig("jpg/Progressiveness_" + workload + "_General.jpg", bbox_inches='tight', transparent=True)
#         plt.savefig("pdf/Progressiveness_" + workload + "_General.pdf", bbox_inches='tight', transparent=True)
#     else:
#         for i in range(len(generic_algo_name)):
#             elapse_time = reader.iloc[i + 8].to_list()[1:]
#             elapse_time.insert(0, 0)
#             plt.plot(elapse_time, ind, '-', markersize=10, color=generic_colors[i],
#                      marker=markers[i], label=generic_algo_name[i])
#         plt.legend(bbox_to_anchor=(0.49, 1.25), loc=9, borderaxespad=0, fontsize=15, ncol=5)
#         # plt.show()
#         plt.savefig("jpg/Progressiveness_" + workload + "_Generic.jpg", bbox_inches='tight', transparent=True)
#         plt.savefig("pdf/Progressiveness_" + workload + "_Generic.pdf", bbox_inches='tight', transparent=True)
#     plt.close()
# def throughput_varying_arr_rate(reader, general):
#     plt.rcParams['font.size'] = 20
#     plt.rc('ytick', labelsize=20)
#     plt.figure(figsize=(10, 4))
#     ind = np.arange(4)
#     plt.xticks([0.3, 1.3, 2.3, 3.3], varying_arr_rate, fontsize=20)
#     if general:
#         print(reader.loc[0:7, ['Algorithm'] + varying_arr_rate])
#         width = 0.1
#         for i in range(len(general_algo_name)):
#             plt.bar(ind + width * i, reader.iloc[i].to_list()[1:5], width, color=general_colors[i],
#                     hatch=hatches[i],
#                     label=general_algo_name[i],
#                     edgecolor="k")
#         plt.ylabel('Tpt.(tuples/second)', fontsize=20)
#         plt.legend(bbox_to_anchor=(0.44, 1.35), loc=9, borderaxespad=0, fontsize=15, ncol=4)
#         # plt.show()
#         plt.savefig("jpg/Throughput_Arrival_General.jpg", bbox_inches='tight', transparent=True)
#         plt.savefig("pdf/Throughput_Arrival_General.pdf", bbox_inches='tight', transparent=True)
#     else:
#         print(reader.loc[8:18, ['Algorithm'] + varying_arr_rate])
#         width = 0.08
#         for i in range(len(generic_algo_name)):
#             plt.bar(ind + width * i, reader.iloc[i + 8].to_list()[1:5], width, color=generic_colors[i],
#                     hatch=hatches[i],
#                     label=generic_algo_name[i],
#                     edgecolor="k")
#         plt.legend(bbox_to_anchor=(0.5, 1.22), loc=9, borderaxespad=0, fontsize=12, ncol=10)
#         # plt.show()
#         plt.savefig("jpg/Throughput_Arrival_Generic.jpg", bbox_inches='tight', transparent=True)
#         plt.savefig("pdf/Throughput_Arrival_Generic.pdf", bbox_inches='tight', transparent=True)


@click.command()
@click.option('--purity', default='raw/purity.csv', show_default=True)
@click.option('--throughput', default='raw/throughput.csv', show_default=True)
@click.option('--config-landmark', default='raw/landmark.csv', show_default=True)
@click.option('--config-sliding', default='raw/sliding.csv', show_default=True)
@click.option('--config-lamda', default='raw/damped.csv', show_default=True)
@click.option('--config-alpha', default='raw/damped.csv', show_default=True)
@click.option('--cmm', default='raw/cmm-eds.csv', show_default=True)
@click.option('--cluster-size-eds', default='mock/window_cluster_size_eds.csv', show_default=True)
@click.option('--outlier-size-eds', default='mock/window_outlier_size_eds.csv', show_default=True)
@click.option('--throughput-eds', default='raw/throughput-eds.csv', show_default=True)
@click.option('--purity-ods', default='raw/purity-ods.csv', show_default=True)
@click.option('--offline', default='mock/offline.csv', show_default=True)
# below are unnecessary currently
@click.option('--dimension', default='mock/dim.csv', show_default=True)
@click.option('--cluster-size-fct', default='mock/window_cluster_size_fct.csv', show_default=True)
@click.option('--prog-fct', default='raw/prog-fct.csv', show_default=True)
@click.option('--prog-kdd', default='raw/prog-kdd.csv', show_default=True)
@click.option('--prog-sensor', default='raw/prog-sensor.csv', show_default=True)
@click.option('--prog-insects', default='raw/prog-insects.csv', show_default=True)
@click.option('--etb-fct', default='raw/etb-fct.csv', show_default=True)
@click.option('--etb-kdd', default='raw/etb-kdd.csv', show_default=True)
@click.option('--etb-sensor', default='raw/etb-sensor.csv', show_default=True)
@click.option('--etb-insects', default='raw/etb-insects.csv', show_default=True)
@click.option('--arr-rate', default='raw/arr-rate.csv', show_default=True)
def draw_all_pictures(purity, throughput, cmm, dimension, offline,
                      cluster_size_fct,cluster_size_eds,outlier_size_eds,
                      prog_fct, prog_kdd, prog_sensor, prog_insects,
                      etb_fct, etb_kdd, etb_sensor, etb_insects, arr_rate, throughput_eds, purity_ods,
                      config_landmark, config_sliding, config_lamda, config_alpha):
    compare_order = [[3, 8, 11, 9, 10, 12], [3, 4, 5], [6, 3, 13, 7, 14], [3, 1, 2]]
    compare_name = [["CFT", "MCs", "CoreT", "Grids", "DPT", "AMS"],
                    ["LandmarkWM", "SlidingWM", "DampedWM"], ["NoOutlierD", "OutlierD-B", "OutlierD-T", "OutlierD", "OutlierD-BT"],
                    ["NoRefine", "Refine [Distance-based]", "Refine [Density-based]"]]
    colors = [[0, 1, 2, 3, 4, 5], [8, 9, 10], [11, 12, 13, 14, 18], [15, 6, 17]]
    if purity != '':
        print('---------Purity Comparison of Design Aspects / Overall on Real-world Workloads---------')
        purity_reader = pd.read_csv(purity)
        # design aspects
        for i in range(4):
            data = []
            for j in range(len(compare_order[i])):
                data.append(purity_reader.iloc[compare_order[i][j] + 7].to_list()[1:5])
            purity_and_throughput(compare_name[i], data, colors[i], True, False, False)

        # overall
        general_and_benne_on_real_data = []
        for i in range(0, 8):
            general_and_benne_on_real_data.append(purity_reader.iloc[i].to_list()[1:5])
        general_and_benne_on_real_data.append(purity_reader.iloc[15].to_list()[1:5])
        general_and_benne_on_real_data.append(purity_reader.iloc[16].to_list()[1:5])
        purity_and_throughput(general_algo_name, general_and_benne_on_real_data, np.arange(1,11), True, True, False)
    if throughput != '':
        print('---------Throughput Comparison of Design Aspects / Overall on Real-world Workloads---------')
        throughput_reader = pd.read_csv(throughput)
        # design aspects
        for i in range(4):
            data = []
            for j in range(len(compare_order[i])):
                data.append(throughput_reader.iloc[compare_order[i][j] + 7].to_list()[1:5])
            purity_and_throughput(compare_name[i], data, colors[i], False, False, False)
        # overall
        general_and_benne_on_real_data = []
        for i in range(0, 8):
            general_and_benne_on_real_data.append(throughput_reader.iloc[i].to_list()[1:5])
        general_and_benne_on_real_data.append(throughput_reader.iloc[15].to_list()[1:5])
        general_and_benne_on_real_data.append(throughput_reader.iloc[16].to_list()[1:5])
        purity_and_throughput(general_algo_name, general_and_benne_on_real_data, np.arange(1, 11), False, True, False)
    if config_landmark != 0:
        print('---------Window length configuration on FCT---------')
        landmark = pd.read_csv(config_landmark)
        config = landmark['landmark'].to_list()[0:]
        purity = landmark['purity'].to_list()[0:]
        throughput = landmark['qps'].to_list()[0:]
        configer(config, purity, throughput, 'Landmark')

        sliding = pd.read_csv(config_sliding)
        config = sliding['sliding'].to_list()[0:]
        purity = sliding['purity'].to_list()[0:]
        throughput = sliding['qps'].to_list()[0:]
        configer(config, purity, throughput, 'Sliding')

        lamda = pd.read_csv(config_lamda)
        config = lamda['lambda'].to_list()[0:10]
        purity = lamda['purity'].to_list()[0:10]
        throughput = lamda['qps'].to_list()[0:10]
        configer(config, purity, throughput, 'lambda')

        alpha = pd.read_csv(config_alpha)
        config = alpha['alpha'].to_list()[10:18]
        purity = alpha['purity'].to_list()[10:18]
        throughput = alpha['qps'].to_list()[10:18]
        configer(config, purity, throughput, 'alpha')
    if cmm != '':
        cmm_reader = pd.read_csv(cmm)
        print('---------CMM Comparison of Data Structure on EDS---------')
        # CMM of Summarizing Data Structure on EDS
        cmm_summarizing_data_structure = []
        for i in range(len(compare_order[0])):
            cmm_summarizing_data_structure.append(cmm_reader.iloc[compare_order[0][i] + 7].to_list()[1:])
        draw_line(compare_name[0], cmm_summarizing_data_structure, colors[0], False, False, False, False, 0)

        print('---------CMM Comparison of Outlier Detection on EDS---------')
        # CMM of Outlier Detection on EDS
        cmm_outlier_detection = []
        for i in range(len(compare_order[2])):
            cmm_outlier_detection.append(cmm_reader.iloc[compare_order[2][i] + 7].to_list()[1:])
        draw_line(compare_name[2], cmm_outlier_detection, colors[2], False, False, False, False, 0)
    if cluster_size_eds != '':
        window_clusters = pd.read_csv(cluster_size_eds)
        print('---------Cluster Size Analysis of Window Models on EDS---------')
        # Cluster Size of Window Model on EDS
        cluster_size_window_eds = []
        order = [0, 1, 2]
        for j in range(len(order)):
            cluster_size_window_eds.append(window_clusters.iloc[order[j]].to_list()[1:])
        draw_line(compare_name[1], cluster_size_window_eds, colors[1], False, False, False, True,1)
    if outlier_size_eds != '':
        window_clusters = pd.read_csv(outlier_size_eds)
        print('---------Outlier Size Analysis of Window Models on EDS---------')
        # Outlier Size of Window Model on EDS
        outlier_size_window_eds = []
        order = [0, 1, 2]
        for j in range(len(order)):
            outlier_size_window_eds.append(window_clusters.iloc[order[j]].to_list()[1:])
        draw_line(compare_name[1], outlier_size_window_eds, colors[1], False, False, False, True,2)
    if throughput_eds != '':
        throughput_reader = pd.read_csv(throughput_eds)
        print('---------Throughput of Window Model on EDS---------')
        # Throughput of Window Model on EDS
        throughput_window_eds = []
        for i in range(len(compare_order[1])):
            throughput_window_eds.append(throughput_reader.iloc[compare_order[1][i] + 7].to_list()[1:])
        draw_line(compare_name[1], throughput_window_eds, colors[1], False, True, False, False,0)

        print('---------Purity of Window Model on ODS---------')
        purity_reader = pd.read_csv(purity_ods)
        # Purity of Window Model on ODS
        purity_window_ods = []
        for i in range(len(compare_order[1])):
            purity_window_ods.append(purity_reader.iloc[compare_order[1][i] + 7].to_list()[1:])
        draw_line(compare_name[1], purity_window_ods, colors[1], True, False, True, False,0)

        print('---------Throughput of Outlier Detection on EDS---------')
        throughput_outlier_eds = []
        # Throughput of Outlier Detection on EDS
        for i in range(len(compare_order[2])):
            throughput_outlier_eds.append(throughput_reader.iloc[compare_order[2][i] + 7].to_list()[1:])
        draw_line(compare_name[2], throughput_outlier_eds, colors[2], False, True, False, False, 0)
    if offline != '':
        print('---------Offline Comparison of StreamKM++ and DenStream---------')
        behavior = pd.read_csv(offline)
        purity = []
        throughput = []
        p = behavior.iloc[:, 3:4].values
        t = behavior.iloc[:, 4:5].values
        purtiy_with = [p[0][0], p[2][0], p[4][0], p[6][0]]
        throughput_with = [t[0][0], t[2][0], t[4][0], t[6][0]]
        purtiy_without = [p[1][0], p[3][0], p[5][0], p[7][0]]
        throughput_without = [t[1][0], t[3][0], t[5][0], t[7][0]]
        # StreamKM++
        purity.append(purtiy_with)
        purity.append(purtiy_without)
        throughput.append(throughput_with)
        throughput.append(throughput_without)

        purtiy_with = [p[8][0], p[10][0], p[12][0], p[14][0]]
        throughput_with = [t[8][0], t[10][0], t[12][0], t[14][0]]
        purtiy_without = [p[9][0], p[11][0], p[13][0], p[15][0]]
        throughput_without = [t[9][0], t[11][0], t[13][0], t[15][0]]
        # DenStream
        purity.append(purtiy_with)
        purity.append(purtiy_without)
        throughput.append(throughput_with)
        throughput.append(throughput_without)

        name = ["StreamKM++ [NoRefine]", "StreamKM++ [Refine]", "DenStream [NoRefine]",
                "DenStream [Refine]"]
        color = [18, 19, 20, 21]

        purity_and_throughput(name, purity, color, True, False, False)
        purity_and_throughput(name, throughput, color, False, False, False)

        # print('---------start drawing CMM of generic algorithms on ODS---------')
        # ods_purity_reader = pd.read_csv(cmm)
        # assert general_algo_name == cmm_reader['Algorithm'].to_list()[0:8]
        # assert generic_algo_name == cmm_reader['Algorithm'].to_list()[8:18]
        # assert (len(cmm_reader.iloc[i].to_list()) == len(cmm_reader.iloc[i + 1].to_list()) for i in range(18))
        # compare_name = [["Landmark", "Sliding", "Damped"]]
        # compare_order = [[3, 4, 5]]
        # for i in range(len(compare_order)):
        #     data = []
        #     for j in range(len(compare_order[i])):
        #         data.append(cmm_reader.iloc[compare_order[i][j] + 7].to_list()[1:])
        #     draw_cmm(compare_name[i], data, colors[i], False)
        # if dimension == '':
        #     dim_reader = pd.read_csv(dimension)
        #     print('---------start drawing dimension of generic algorithms on FCT and Insects---------')
        #     order = [0, 1, 2, 3, 4, 5]
        #     data = []
        #     for j in range(len(order)):
        #         data.append(dim_reader.iloc[compare_order[j]].to_list()[1:3])
        #     purity_and_throughput(compare_name[0], data, colors[0], False, True, False, True)
        #     # prinzt('---------start drawing purity of generic algorithms on synthetic workloads---------')
        #     # purity_and_throughput(purity_reader, True, False, False)

    # if cluster_size_fct != '':
    #     window_clusters = pd.read_csv(cluster_size_fct)
    #     print('---------Cluster Size Analysis on FCT---------')
    #     cluster_size = []
    #     order = [0,1,2]
    #     for j in range(len(order)):
    #         cluster_size.append(window_clusters.iloc[order[j]].to_list()[1:])
    #     draw_line(compare_name[1], cluster_size, colors[1], False, False, False, True,0)
    # if prog_fct != '':
    #     p_fct_reader = pd.read_csv(prog_fct)
    #     assert general_algo_name == p_fct_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == p_fct_reader['Algorithm'].to_list()[8:18]
    #     assert (len(p_fct_reader.iloc[i].to_list()) == len(p_fct_reader.iloc[i + 1].to_list()) for i in range(18))
    #     print('---------start drawing Progressiveness of general algorithms on CoverType---------')
    #     progressiveness(p_fct_reader, True, 'FCT')
    #     print('---------start drawing Progressiveness of generic algorithms on CoverType---------')
    #     progressiveness(p_fct_reader, False, 'FCT')
    #
    # if prog_kdd != '':
    #     p_kdd_reader = pd.read_csv(prog_kdd)
    #     assert general_algo_name == p_kdd_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == p_kdd_reader['Algorithm'].to_list()[8:18]
    #     assert (len(p_kdd_reader.iloc[i].to_list()) == len(p_kdd_reader.iloc[i + 1].to_list()) for i in range(18))
    #     print('---------start drawing Progressiveness of general algorithms on KDD99---------')
    #     progressiveness(p_kdd_reader, True, 'KDD99')
    #     print('---------start drawing Progressiveness of generic algorithms on KDD99---------')
    #     progressiveness(p_kdd_reader, False, 'KDD99')
    #
    # if prog_sensor != '':
    #     p_sensor_reader = pd.read_csv(prog_sensor)
    #     assert general_algo_name == p_sensor_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == p_sensor_reader['Algorithm'].to_list()[8:18]
    #     assert (len(p_sensor_reader.iloc[i].to_list()) == len(p_sensor_reader.iloc[i + 1].to_list()) for i in range(18))
    #     print('---------start drawing Progressiveness of general algorithms on Sensor---------')
    #     progressiveness(p_sensor_reader, True, 'Sensor')
    #     print('---------start drawing Progressiveness of generic algorithms on Sensor---------')
    #     progressiveness(p_sensor_reader, False, 'Sensor')
    #
    # if prog_insects != '':
    #     p_insects_reader = pd.read_csv(prog_insects)
    #     assert general_algo_name == p_insects_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == p_insects_reader['Algorithm'].to_list()[8:18]
    #     assert (len(p_insects_reader.iloc[i].to_list()) == len(p_insects_reader.iloc[i + 1].to_list()) for i in
    #             range(18))
    #     print('---------start drawing Progressiveness of general algorithms on Insects---------')
    #     progressiveness(p_insects_reader, True, 'Insects')
    #     print('---------start drawing Progressiveness of generic algorithms on Insects---------')
    #     progressiveness(p_insects_reader, False, 'Insects')
    #
    # if etb_fct != '':
    #     etb_fct_reader = pd.read_csv(etb_fct)
    #     assert general_algo_name == etb_fct_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == etb_fct_reader['Algorithm'].to_list()[8:18]
    #     assert (len(etb_fct_reader.iloc[i].to_list()) == len(etb_fct_reader.iloc[i + 1].to_list()) for i in
    #             range(18))
    #     assert design_aspects == etb_fct_reader.columns.tolist()[1:]
    #     print('---------start drawing Execution Time Breakdown of general algorithms on CoverType---------')
    #     execution_time_breakdown(etb_fct_reader, True, 'FCT')
    #     print('---------start drawing Execution Time Breakdown of generic algorithms on CoverType---------')
    #     execution_time_breakdown(etb_fct_reader, False, 'FCT')
    #
    # if etb_kdd != '':
    #     etb_kdd_reader = pd.read_csv(etb_kdd)
    #     assert general_algo_name == etb_kdd_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == etb_kdd_reader['Algorithm'].to_list()[8:18]
    #     assert (len(etb_kdd_reader.iloc[i].to_list()) == len(etb_kdd_reader.iloc[i + 1].to_list()) for i in
    #             range(18))
    #     assert design_aspects == etb_kdd_reader.columns.tolist()[1:]
    #     print('---------start drawing Execution Time Breakdown of general algorithms on KDD---------')
    #     execution_time_breakdown(etb_kdd_reader, True, 'KDD')
    #     print('---------start drawing Execution Time Breakdown of generic algorithms on KDD---------')
    #     execution_time_breakdown(etb_kdd_reader, False, 'KDD')
    #
    # if etb_sensor != '':
    #     etb_sensor_reader = pd.read_csv(etb_sensor)
    #     assert general_algo_name == etb_sensor_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == etb_sensor_reader['Algorithm'].to_list()[8:18]
    #     assert (len(etb_sensor_reader.iloc[i].to_list()) == len(etb_sensor_reader.iloc[i + 1].to_list()) for i in
    #             range(18))
    #     assert design_aspects == etb_sensor_reader.columns.tolist()[1:]
    #     print('---------start drawing Execution Time Breakdown of general algorithms on Sensor---------')
    #     execution_time_breakdown(etb_sensor_reader, True, 'Sensor')
    #     print('---------start drawing Execution Time Breakdown of generic algorithms on Sensor---------')
    #     execution_time_breakdown(etb_sensor_reader, False, 'Sensor')
    #
    # if etb_insects != '':
    #     etb_insects_reader = pd.read_csv(etb_insects)
    #     assert general_algo_name == etb_insects_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == etb_insects_reader['Algorithm'].to_list()[8:18]
    #     assert (len(etb_insects_reader.iloc[i].to_list()) == len(etb_insects_reader.iloc[i + 1].to_list()) for i in
    #             range(18))
    #     assert design_aspects == etb_insects_reader.columns.tolist()[1:]
    #     print('---------start drawing Execution Time Breakdown of general algorithms on INSECTS---------')
    #     execution_time_breakdown(etb_insects_reader, True, 'INSECTS')
    #     print('---------start drawing Execution Time Breakdown of generic algorithms on INSECTS---------')
    #     execution_time_breakdown(etb_insects_reader, False, 'INSECTS')
    #
    # if arr_rate != '':
    #     arr_rate_reader = pd.read_csv(arr_rate)
    #     assert general_algo_name == arr_rate_reader['Algorithm'].to_list()[0:8]
    #     assert generic_algo_name == arr_rate_reader['Algorithm'].to_list()[8:18]
    #     assert (len(arr_rate_reader.iloc[i].to_list()) == len(arr_rate_reader.iloc[i + 1].to_list()) for i in
    #             range(18))
    #     assert varying_arr_rate == arr_rate_reader.columns.tolist()[1:]
    #     print('---------start drawing throughput under varying arrival rate of general algorithms on EDS---------')
    #     throughput_varying_arr_rate(arr_rate_reader, True)
    #     print('---------start drawing throughput under varying arrival rate of generic algorithms on EDS---------')
    #     throughput_varying_arr_rate(arr_rate_reader, False)

if __name__ == '__main__':
    folder = os.getcwd()
    if not os.path.exists(folder + '/jpg'):
        os.makedirs(folder + '/jpg')
    if not os.path.exists(folder + '/pdf'):
        os.makedirs(folder + '/pdf')
    draw_all_pictures()
