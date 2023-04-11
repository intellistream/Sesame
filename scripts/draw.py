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
synthetic_workloads = ['EDS', 'ODS']
varying_arr_rate = ['2k/s', '20k/s', '50k/s', '100k/s']
design_aspects = ['WindowModel', 'OutlierDetection',
                  'DataStructure', 'OfflineRefinement']
general_algo_name = ['Birch', 'StreamKM++', 'CluStream', 'DenStream', 'DBStream', 'EDMStream', 'DStream', 'SL-KMeans',
                     'Benne (Accuracy)', 'Benne (Efficiency)']
generic_algo_name = ['G1', 'G2', 'G3', 'G4',
                     'G5', 'G6', 'G7', 'G8', 'G9', 'G10']
general_colors = ['orange', 'gold', 'yellowgreen', 'grey',
                  'seagreen', 'cornflowerblue', 'crimson', 'blue']
generic_colors = ['salmon', 'peru', 'lawngreen', 'turquoise', 'mediumslateblue', 'cornflowerblue', 'violet', 'hotpink',
                  'silver', 'darkorange', 'slategrey', 'chocolate', 'lime', 'royalblue', 'plum', 'linen', 'wheat',
                  'powderblue', "darkorange", "bisque", "cyan", "lightcyan"]
etb_colors = ['orange', 'cornflowerblue', 'green', 'maroon']
hatches = ['/', '-', 'o', 'x', '+', '.', '*', '|', 'O', '\\']
markers = ['o', 'x', 'd', '*', 's', '^', 'p', '<', 'H', 'P']


# 2,3,5,6
def purity_and_throughput(legend_name, data, colors, purity, general, dimension):
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
            start = ind + width * i / 2 - 0.01
        elif len(data) == 2:
            w = width
            start = ind + 0.15 + width * i
        elif len(data) == 3:
            w = width
            start = ind + 0.05 + width * i
        elif len(data) == 4:
            w = width / 1.3
            start = ind + width * i / 1.3 + 0.01
        elif len(data) == 5:
            w = width / 1.5
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
            plt.savefig("jpg/Purity_Real_General " + str(name) +
                        ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Purity_Real_General " + str(name) +
                        ".pdf", bbox_inches='tight', transparent=True)
        else:
            plt.savefig("jpg/Purity_Real_Generic " + str(name) +
                        ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Purity_Real_Generic " + str(name) +
                        ".pdf", bbox_inches='tight', transparent=True)
    elif dimension:
        plt.yticks([5, 20, 40, 60, 80])
        plt.ylabel("Proportion (%)", fontsize=18)
        plt.grid(axis='y', linestyle='--', linewidth=0.5)
        # plt.legend(bbox_to_anchor=(0.5, 1.42), loc=9, borderaxespad=0, fontsize=14, ncol=3)
        # plt.show()
        plt.savefig("jpg/Dim_FCT_INSECTS " + str(name) + ".jpg",
                    bbox_inches='tight', transparent=True)
        plt.savefig("pdf/Dim_FCT_INSECTS " + str(name) + ".pdf",
                    bbox_inches='tight', transparent=True)
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
            plt.savefig("jpg/Throughput_Real_General " + str(name) +
                        ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Throughput_Real_General " + str(name) +
                        ".pdf", bbox_inches='tight', transparent=True)
        else:
            plt.savefig("jpg/Throughput_Real_Generic " + str(name) +
                        ".jpg", bbox_inches='tight', transparent=True)
            plt.savefig("pdf/Throughput_Real_Generic " + str(name) +
                        ".pdf", bbox_inches='tight', transparent=True)
    plt.close()


def draw_line(legend_name, data, colors, ods, throughput, purity, window, id):
    plt.figure(figsize=(10, 4))
    plt.rcParams['font.size'] = 36
    if throughput:
        # data = np.log(data)
        file_name = "Throughput_"
        plt.ylabel('Throughput', fontsize=36)
        ax = plt.gca()
        if window == False:
            ax.ticklabel_format(style='sci', scilimits=(-1, 1), axis='y')
        else:
            ax.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
        legend_position = (0.45, 1.26)
    elif purity and window == False:
        file_name = "Purity_"
        plt.ylabel('Purity', fontsize=36)
        plt.ylim(0.75, 0.95)
        legend_position = (0.45, 1.25)
    elif purity and window == True:
        file_name = "Purity_"
        plt.ylabel('Purity', fontsize=36)
        plt.ylim(0, 0.85)
        legend_position = (0.45, 1.25)
    elif window:
        file_name = "Window_"
        plt.ylabel(
            'Cluster Number(x' + '$ \mathit{10}^{1}$' + ')', fontsize=30)
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
        ind = np.array([0, 2, 4, 6, 8, 10, 12, 14, 16, 18])
        plt.xlabel('Length of the stream (x' +
                   '$ \mathit{10}^{4}$' + ')', fontsize=36)
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
        plt.savefig("jpg/" + file_name + "Generic " + str(legend_name) +
                    ".jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/" + file_name + "Generic " + str(legend_name) +
                    ".pdf", bbox_inches='tight', transparent=True)
    else:
        for i in range(len(legend_name)):
            plt.plot(ind, data[i], '-', markersize=15, color=generic_colors[colors[i]],
                     marker=markers[i], label=legend_name[i])
        if show_legend:
            plt.legend(bbox_to_anchor=(0.5, 1.80), loc=9, borderaxespad=0, fontsize=24, ncol=6, frameon=True,
                       framealpha=1, edgecolor='black')
        # plt.show()
        if len(data) == 5:
            legend_name = '_Timer'
        plt.savefig("jpg/" + file_name + "Generic " + str(legend_name) +
                    ".jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/" + file_name + "Generic " + str(legend_name) +
                    ".pdf", bbox_inches='tight', transparent=True)
    plt.close()


def configer(config, purity, throughput, measurement, id):
    fig = plt.figure(figsize=(12.5, 7.5))
    plt.rcParams['font.size'] = 36
    ax1 = fig.add_subplot(111)
    # plt.grid(axis='y', linestyle='--', linewidth=0.5)
    ax1.plot(config, purity, linestyle='-', marker='s',
             markersize=18, color='b', label='Purity')
    ax1.set_ylabel('Purity', fontsize=36)
    plt.xlabel(measurement, fontsize=36)
    plt.tick_params(axis='y', size=35)
    plt.yticks(np.arange(min(purity), max(purity) + (max(purity) - min(purity)) / 4, (max(purity) - min(purity)) / 4),
               size=35)
    if id <= 2:
        plt.xticks(np.arange(50000, 500000, 100000), size=25)
    elif id == 3:
        plt.xticks(np.arange(1.1, 2.9, 0.2), size=25)
    elif id == 4:
        plt.xticks(np.arange(0.2, 3.0, 0.4), size=25)
    if show_legend:
        plt.legend(bbox_to_anchor=(0.13, 1.23), loc='upper left', fontsize=24, frameon=True, framealpha=1,
                   edgecolor='black')

    ax2 = ax1.twinx()  # this is the important function
    ax2.plot(config, throughput, linestyle='-', color='r',
             marker='^', markersize=18, label='Throughput')
    # ax2.set_xlim([0, np.e])
    ax2.set_ylabel('Throughput', fontsize=36)
    plt.tick_params(axis='y', size=36)
    y_magnitude = math.floor(math.log10(max(throughput)))
    y_magnitude = math.pow(10, y_magnitude)
    interval = np.ceil(round((max(throughput) - min(throughput))/ y_magnitude, 1) * y_magnitude) / 4
    # if id == 2:
    #     interval += 250
    # if id == 4:
    #     interval += 5000
    y_tick = np.arange(np.ceil(round(min(throughput) / y_magnitude, 1) * y_magnitude),
                       np.ceil(round(max(throughput) / y_magnitude, 1) * y_magnitude) + interval, interval)
    plt.yticks(y_tick, size=35)
    plt.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
    ax1.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))
    # ax2.yaxis.set_major_formatter(mtick.FormatStrFormatter('%.2f'))

    # 图例
    if show_legend:
        plt.legend(bbox_to_anchor=(0.9, 1.23), loc='upper right', fontsize=24, frameon=True, framealpha=1,
                   edgecolor='black')
    # plt.show()
    plt.savefig("jpg/Config_" + measurement + ".jpg",
                bbox_inches='tight', transparent=True)
    plt.savefig("pdf/Config_" + measurement + ".pdf",
                bbox_inches='tight', transparent=True)
    plt.close()


@click.command()
@click.option('--purity', default='raw/purity.csv', show_default=True)
@click.option('--throughput', default='raw/throughput.csv', show_default=True)
@click.option('--config-landmark', default='raw/landmark.csv', show_default=True)
@click.option('--config-sliding', default='raw/sliding.csv', show_default=True)
@click.option('--config-lamda', default='raw/damped.csv', show_default=True)
@click.option('--config-alpha', default='raw/damped.csv', show_default=True)
@click.option('--cmm', default='raw/cmm-eds.csv', show_default=True)
@click.option('--throughput-eds', default='raw/throughput-eds.csv', show_default=True)
@click.option('--purity-ods', default='raw/purity-ods.csv', show_default=True)
@click.option('--throughput-ods', default='raw/throughput-ods.csv', show_default=True)
@click.option('--offline', default='raw/offline.csv', show_default=True)
# below are unnecessary currently
def draw_all_pictures(purity, throughput, cmm, offline,
                      throughput_eds, purity_ods, throughput_ods,
                      config_landmark, config_sliding, config_lamda, config_alpha):
    compare_order = [[8, 3, 15, 9, 10, 14], [
        3, 4, 5], [6, 12, 11, 3, 13], [3, 1, 2]]
    # compare_order = [[3, 8, 11, 9, 10, 12], [3, 4, 5], [6, 3, 13, 7, 14], [3, 1, 2]]
    compare_name = [["CFT", "MCs", "CoreT", "Grids", "DPT", "AMS"],
                    ["LandmarkWM", "SlidingWM", "DampedWM"],
                    ["NoOutlierD", "OutlierD-B", "OutlierD-T",
                     "OutlierD", "OutlierD-BT"],
                    ["NoRefine", "Refine [Distance-based]", "Refine [Density-based]"]]
    colors = [[0, 1, 2, 3, 4, 5], [8, 9, 10],
              [11, 12, 13, 14, 18], [15, 6, 17]]
    if purity != '':
        print('---------Purity Comparison of Design Aspects / Overall on Real-world Workloads---------')
        purity_reader = pd.read_csv(purity)
        # design aspects
        for i in range(4):
            data = []
            for j in range(len(compare_order[i])):
                data.append(
                    purity_reader.iloc[compare_order[i][j] + 7].to_list()[1:5])
            purity_and_throughput(
                compare_name[i], data, colors[i], True, False, False)

        # # overall
        general_and_benne_on_real_data = []
        for i in range(0, 8):
            general_and_benne_on_real_data.append(
                purity_reader.iloc[i].to_list()[1:5])
        general_and_benne_on_real_data.append(
            purity_reader.iloc[20].to_list()[1:5])
        general_and_benne_on_real_data.append(
            purity_reader.iloc[16].to_list()[1:5])
        purity_and_throughput(general_algo_name, general_and_benne_on_real_data, np.arange(
            1, 11), True, True, False)

    if throughput != '':
        print('---------Throughput Comparison of Design Aspects / Overall on Real-world Workloads---------')
        throughput_reader = pd.read_csv(throughput)
        # design aspects
        for i in range(4):
            data = []
            for j in range(len(compare_order[i])):
                data.append(
                    throughput_reader.iloc[compare_order[i][j] + 7].to_list()[1:5])
            purity_and_throughput(
                compare_name[i], data, colors[i], False, False, False)
        # # overall
        general_and_benne_on_real_data = []
        for i in range(0, 8):
            general_and_benne_on_real_data.append(
                throughput_reader.iloc[i].to_list()[1:5])
        general_and_benne_on_real_data.append(
            throughput_reader.iloc[20].to_list()[1:5])
        general_and_benne_on_real_data.append(
            throughput_reader.iloc[16].to_list()[1:5])
        purity_and_throughput(general_algo_name, general_and_benne_on_real_data, np.arange(
            1, 11), False, True, False)

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

        name = ["DenStream [NoRefine]",
                "DenStream [Refine]", "StreamKM++ [NoRefine]", "StreamKM++ [Refine]"]
        color = [18, 19, 20, 21]

        purity_and_throughput(name, purity, color, True, False, False)
        purity_and_throughput(name, throughput, color, False, False, False)

    if cmm != '':
        cmm_reader = pd.read_csv(cmm)
        print('---------CMM Comparison of Data Structure on EDS---------')
        # CMM of Summarizing Data Structure on EDS
        cmm_summarizing_data_structure = []
        for i in range(len(compare_order[0])):
            cmm_summarizing_data_structure.append(
                cmm_reader.iloc[compare_order[0][i] + 7].to_list()[1:])
        draw_line(compare_name[0], cmm_summarizing_data_structure,
                  colors[0], False, False, False, False, 0)

        # print('---------CMM Comparison of Outlier Detection on EDS---------')
        # # CMM of Outlier Detection on EDS
        # cmm_outlier_detection = []
        # for i in range(len(compare_order[2])):
        #     cmm_outlier_detection.append(
        #         cmm_reader.iloc[compare_order[2][i] + 7].to_list()[1:])
        # draw_line(compare_name[2], cmm_outlier_detection,
        #           colors[2], False, False, False, False, 0)

    if throughput_eds != '':
        throughput_reader_EDS = pd.read_csv(throughput_eds)
        throughput_reader_ODS = pd.read_csv(throughput_ods)
        print('---------Throughput of Window Model on EDS---------')
        # Throughput of Window Model on EDS
        throughput_window_eds = []
        for i in range(len(compare_order[1])):
            throughput_window_eds.append(throughput_reader_EDS.iloc[compare_order[1][i] + 7].to_list()[1:])
        draw_line(compare_name[1], throughput_window_eds, colors[1], False, True, False, False, 0)
        #
        # print('---------Throughput of Data Structure on EDS---------')
        # throughput_ds_eds = []
        # for i in range(len(compare_order[0])):
        #     throughput_ds_eds.append(throughput_reader_EDS.iloc[compare_order[0][i] + 7].to_list()[1:])
        # draw_line(compare_name[0], throughput_ds_eds, colors[0], False, True, False, False, 0)
        #
        print('---------Purity of Window Model on ODS---------')
        purity_reader = pd.read_csv(purity_ods)
        # Purity of Window Model on ODS
        purity_window_ods = []
        for i in range(len(compare_order[1])):
            purity_window_ods.append(
                purity_reader.iloc[compare_order[1][i] + 7].to_list()[1:])
        draw_line(compare_name[1], purity_window_ods,
                  colors[1], True, False, True, True, 0)

        print('---------Throughput of Outlier Detection on ODS---------')
        throughput_outlier_eds = []
        # Throughput of Outlier Detection on EDS
        for i in range(len(compare_order[2])):
            throughput_outlier_eds.append(throughput_reader_ODS.iloc[compare_order[2][i] + 7].to_list()[1:])
        draw_line(compare_name[2], throughput_outlier_eds, colors[2], True, True, False, False, 0)

        print('---------Purity of Outlier Detection on ODS---------')
        purity_outlier_ods = []
        # Throughput of Outlier Detection on EDS
        for i in range(len(compare_order[2])):
            purity_outlier_ods.append(
                purity_reader.iloc[compare_order[2][i] + 7].to_list()[1:])
        draw_line(compare_name[2], purity_outlier_ods,
                  colors[2], True, False, True, False, 0)

    if config_landmark != 0:
        print('---------Window length configuration on FCT---------')
        landmark = pd.read_csv(config_landmark)
        config = landmark['landmark'].to_list()[0:]
        purity = landmark['purity'].to_list()[0:]
        throughput = landmark['qps'].to_list()[0:]
        purity.reverse()
        throughput.reverse()
        configer(config, purity, throughput, 'Landmark', 1)

        sliding = pd.read_csv(config_sliding)
        config = sliding['sliding'].to_list()[0:]
        purity = sliding['purity'].to_list()[0:]
        throughput = sliding['qps'].to_list()[0:]
        configer(config, purity, throughput, 'Sliding', 2)

        lamda = pd.read_csv(config_lamda)
        config = lamda['lambda'].to_list()[0:14]
        purity = lamda['purity'].to_list()[0:14]
        throughput = lamda['qps'].to_list()[0:14]
        configer(config, purity, throughput, 'lambda', 4)

        alpha = pd.read_csv(config_alpha)
        config = alpha['alpha'].to_list()[15:23]
        purity = alpha['purity'].to_list()[15:23]
        throughput = alpha['qps'].to_list()[15:23]
        configer(config, purity, throughput, 'alpha', 3)


if __name__ == '__main__':
    folder = os.getcwd()
    if not os.path.exists(folder + '/jpg'):
        os.makedirs(folder + '/jpg')
    if not os.path.exists(folder + '/pdf'):
        os.makedirs(folder + '/pdf')
    draw_all_pictures()
