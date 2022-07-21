#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import click
import os

from typing import List

real_world_workloads = ["CoverType", "KDD99", "Sensor", "Insects"]
synthetic_workloads = ['EDS', 'ODS']
design_aspects = ['WindowModel', 'OutlierDetection', 'DataStructure', 'OfflineRefinement']
general_algo_name = ['Birch', 'StreamKMeans', 'CluStream', 'DenStream', 'DBStream', 'EDMStream', 'DStream', 'SLKMeans']
generic_algo_name = ['G1', 'G2', 'G3', 'G4', 'G5', 'G6', 'G7', 'G8', 'G9', 'G10']
general_colors = ['orange', 'gold', 'yellowgreen', 'grey', 'seagreen', 'cornflowerblue', 'crimson', 'blue']
generic_colors = ['salmon', 'peru', 'lawngreen', 'turquoise', 'mediumslateblue', 'cornflowerblue', 'violet', 'hotpink',
                  'silver', 'darkorange']
etb_colors = ['orange', 'cornflowerblue', 'green', 'maroon']
hatches = ['///', '---', 'ooo', 'xxx', '+++', '.', '***', '|||', 'O', '\\']
markers = ['o', 'x', 'd', '*', 's', '^', 'p', '<', 'H', 'P']


# functions to adjust the ysticks of throughput picture
# def adjust_throughput_axis(max_value):
#     times = 1
#     while(True):
#         if max_value < 100:
#             max_value = int(np.round(max_value))
#             break
#         else:
#             times = times + 1
#             max_value = max_value / 100
#     throughput_range = [0]
#     max_value = max_value * 100
#     for i in range(1, times):
#         throughput_range.append(max_value)
#         max_value = max_value * 100
#     return throughput_range

def purity_and_throughput(reader, purity, real, general):
    plt.rcParams['font.size'] = 20
    plt.rc('ytick', labelsize=20)
    if real:
        plt.figure(figsize=(10, 4))
        ind = np.arange(4)
        plt.xticks([0.3, 1.3, 2.3, 3.3], real_world_workloads, fontsize=20)
        if general:
            print(reader.loc[0:7, ['Algorithm'] + real_world_workloads])
            width = 0.1
            for i in range(len(general_algo_name)):
                plt.bar(ind + width * i, reader.iloc[i].to_list()[1:5], width, color=general_colors[i],
                        hatch=hatches[i],
                        label=general_algo_name[i],
                        edgecolor="k")
            if purity:
                plt.ylabel('Purity', fontsize=20)
                plt.legend(bbox_to_anchor=(0.44, 1.25), loc=9, borderaxespad=0, fontsize=15, ncol=4)
                # plt.show()
                plt.savefig("jpg/Purity_Real_General.jpg", bbox_inches='tight', transparent=True)
                plt.savefig("pdf/Purity_Real_General.pdf", bbox_inches='tight', transparent=True)
            else:
                plt.ylabel('Tpt.(tuples/second)', fontsize=20)
                plt.legend(bbox_to_anchor=(0.44, 1.35), loc=9, borderaxespad=0, fontsize=15, ncol=4)
                # plt.show()
                plt.savefig("pdf/Throughput_Real_General.pdf", bbox_inches='tight', transparent=True)
                plt.savefig("jpg/Throughput_Real_General.jpg", bbox_inches='tight', transparent=True)
        else:
            print(reader.loc[8:18, ['Algorithm'] + real_world_workloads])
            width = 0.08
            for i in range(len(generic_algo_name)):
                plt.bar(ind + width * i, reader.iloc[i + 8].to_list()[1:5], width, color=generic_colors[i],
                        hatch=hatches[i],
                        label=generic_algo_name[i],
                        edgecolor="k")
            if purity:
                plt.legend(bbox_to_anchor=(0.5, 1.12), loc=9, borderaxespad=0, fontsize=12, ncol=10)
                # plt.show()
                plt.savefig("jpg/Purity_Real_Generic.jpg", bbox_inches='tight', transparent=True)
                plt.savefig("pdf/Purity_Real_Generic.pdf", bbox_inches='tight', transparent=True)
            else:
                plt.legend(bbox_to_anchor=(0.5, 1.22), loc=9, borderaxespad=0, fontsize=12, ncol=10)
                # plt.show()
                plt.savefig("jpg/Throughput_Real_Generic.jpg", bbox_inches='tight', transparent=True)
                plt.savefig("pdf/Throughput_Real_Generic.pdf", bbox_inches='tight', transparent=True)
    else:
        plt.figure(figsize=(8, 4))
        ind = np.arange(2)
        plt.xticks([0.3, 1.3], synthetic_workloads, fontsize=20)
        if general:
            print(reader.loc[0:7, ['Algorithm'] + synthetic_workloads])
            width = 0.1
            for i in range(len(general_algo_name)):
                plt.bar(ind + width * i, reader.iloc[i].to_list()[5:7], width, color=general_colors[i],
                        hatch=hatches[i],
                        label=general_algo_name[i],
                        edgecolor="k")
            if purity:
                plt.ylabel('Purity', fontsize=20)
                plt.legend(loc=1, bbox_to_anchor=(1.01, 1.25), ncol=4, fontsize=12)
                # plt.show()
                plt.savefig("jpg/Purity_Synthetic_General.jpg", bbox_inches='tight', transparent=True)
                plt.savefig("pdf/Purity_Synthetic_General.pdf", bbox_inches='tight', transparent=True)
            else:
                plt.legend(loc=1, bbox_to_anchor=(1.01, 1.35), ncol=4, fontsize=12)
                # plt.show()
                plt.savefig("pdf/Throughput_Synthetic_General.pdf", bbox_inches='tight', transparent=True)
                plt.savefig("jpg/Throughput_Synthetic_General.jpg", bbox_inches='tight', transparent=True)
        else:
            print(reader.loc[8:18, ['Algorithm'] + synthetic_workloads])
            width = 0.08
            for i in range(len(generic_algo_name)):
                plt.bar(ind + width * i, reader.iloc[i + 8].to_list()[5:7], width, color=generic_colors[i],
                        hatch=hatches[i],
                        label=generic_algo_name[i],
                        edgecolor="k")
            if purity:
                plt.legend(bbox_to_anchor=(0.5, 1.25), loc=9, borderaxespad=0, fontsize=15, ncol=5)
                # plt.show()
                plt.savefig("jpg/Purity_Synthetic_Generic.jpg", bbox_inches='tight', transparent=True)
                plt.savefig("pdf/Purity_Synthetic_Generic.pdf", bbox_inches='tight', transparent=True)
            else:
                plt.legend(bbox_to_anchor=(0.5, 1.35), loc=9, borderaxespad=0, fontsize=15, ncol=5)
                # plt.show()
                plt.savefig("jpg/Throughput_Synthetic_Generic.jpg", bbox_inches='tight', transparent=True)
                plt.savefig("pdf/Throughput_Synthetic_Generic.pdf", bbox_inches='tight', transparent=True)
    plt.close()


def cmm(reader, general):
    plt.figure(figsize=(12, 4))
    ind = np.arange(5)
    plt.xlabel('Phases of EDS Stream', fontsize=18)
    plt.ylabel('CMM', fontsize=18)
    plt.yticks(size=18)
    plt.xticks(size=18)
    plt.ylim(0.4, 1)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    if general:
        for i in range(len(general_algo_name)):
            plt.plot(ind, reader.iloc[i].to_list()[1:], '-', markersize=10, color=general_colors[i],
                     marker=markers[i], label=general_algo_name[i])
        plt.legend(bbox_to_anchor=(0.5, 1.25), loc=9, borderaxespad=0, fontsize=16, ncol=4)
        # plt.show()
        plt.savefig("jpg/CMM_EDS_General.jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/CMM_EDS_General.pdf", bbox_inches='tight', transparent=True)
    else:
        for i in range(len(generic_algo_name)):
            plt.plot(ind, reader.iloc[i + 8].to_list()[1:], '-', markersize=10, color=generic_colors[i],
                     marker=markers[i], label=generic_algo_name[i])
        plt.legend(bbox_to_anchor=(0.45, 1.15), loc=9, borderaxespad=0, fontsize=12, ncol=10)
        # plt.show()
        plt.savefig("jpg/CMM_EDS_Generic.jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/CMM_EDS_Generic.pdf", bbox_inches='tight', transparent=True)
    plt.close()


def progressiveness(reader, general, workload):
    plt.figure(figsize=(8, 4))
    ind = np.arange(0, 6)
    plt.rcParams['font.size'] = 18
    plt.xlabel('Elapsed Time (usec)', fontsize=18)
    plt.ylabel('Cumulative Percent', fontsize=18)
    plt.yticks(ind, ('0', '20%', '40%', '60%', '80%', ' 100%'), size=18)
    plt.xticks(size=18)
    plt.grid(axis='y', linestyle='--', linewidth=0.5)
    if general:
        for i in range(len(general_algo_name)):
            elapse_time = reader.iloc[i].to_list()[1:]
            elapse_time.insert(0, 0)
            plt.plot(elapse_time, ind, '-', markersize=10, color=general_colors[i],
                     marker=markers[i], label=general_algo_name[i])
        plt.legend(bbox_to_anchor=(0.44, 1.2), loc=9, borderaxespad=0, fontsize=12, ncol=4)
        # plt.show()
        plt.savefig("jpg/Progressiveness_" + workload + "_General.jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/Progressiveness_" + workload + "_General.pdf", bbox_inches='tight', transparent=True)
    else:
        for i in range(len(generic_algo_name)):
            elapse_time = reader.iloc[i + 8].to_list()[1:]
            elapse_time.insert(0, 0)
            plt.plot(elapse_time, ind, '-', markersize=10, color=generic_colors[i],
                     marker=markers[i], label=generic_algo_name[i])
        plt.legend(bbox_to_anchor=(0.49, 1.25), loc=9, borderaxespad=0, fontsize=15, ncol=5)
        # plt.show()
        plt.savefig("jpg/Progressiveness_" + workload + "_Generic.jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/Progressiveness_" + workload + "_Generic.pdf", bbox_inches='tight', transparent=True)
    plt.close()


def execution_time_breakdown(reader, general, workload):
    plt.figure(figsize=(5, 3))
    plt.rcParams['font.size'] = '11'
    plt.ylabel('Percentage(%)', fontsize=11)
    plt.rc('ytick', labelsize=11)
    width = 0.35
    Bottom = reader[design_aspects[0]].to_list()
    Center1 = reader[design_aspects[1]].to_list()
    Center2 = reader[design_aspects[2]].to_list()
    Top = reader[design_aspects[3]].to_list()
    AccumulateCenter1 = [];
    AccumulateCenter2 = []
    for i in range(0, len(Bottom)):
        sum = Bottom[i] + Center1[i]
        AccumulateCenter1.append(sum)
    for i in range(0, len(AccumulateCenter1)):
        sum = AccumulateCenter1[i] + Center2[i]
        AccumulateCenter2.append(sum)
    if general:
        ind = np.arange(len(general_algo_name))
        plt.xticks(ind, general_algo_name, rotation=25, size=11)
        plt.bar(ind, Bottom[0:8], width, label="Window Model", color=etb_colors[0], hatch=hatches[3])
        plt.bar(ind, AccumulateCenter1[0:8], width, label="Outlier Detection", bottom=Bottom[0:8], color=etb_colors[1],
                hatch=hatches[1])
        plt.bar(ind, AccumulateCenter2[0:8], width, label="Data Structure", bottom=AccumulateCenter1[0:8],
                color=etb_colors[2],
                hatch=hatches[2])
        plt.bar(ind, Top[0:8], width, label="Offline Refinement", bottom=AccumulateCenter2[0:8], color=etb_colors[3],
                hatch=hatches[0])
        plt.legend(loc=1, bbox_to_anchor=(1.02, 1.28), ncol=2, prop={'size': 11})
        # plt.show()
        plt.savefig("jpg/ETB_" + workload + "_General.jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/ETB_" + workload + "_General.pdf", bbox_inches='tight', transparent=True)
    else:
        ind = np.arange(len(generic_algo_name))
        plt.xticks(ind, generic_algo_name, rotation=25, size=11)
        plt.xticks(ind, generic_algo_name, rotation=25, size=11)
        plt.bar(ind, Bottom[8:18], width, label=design_aspects[0], color=etb_colors[0], hatch=hatches[3])
        plt.bar(ind, AccumulateCenter1[8:18], width, label=design_aspects[1], bottom=Bottom[8:18], color=etb_colors[1],
                hatch=hatches[1])
        plt.bar(ind, AccumulateCenter2[8:18], width, label=design_aspects[2], bottom=AccumulateCenter1[8:18],
                color=etb_colors[2],
                hatch=hatches[2])
        plt.bar(ind, Top[8:18], width, label=design_aspects[3], bottom=AccumulateCenter2[8:18], color=etb_colors[3],
                hatch=hatches[0])
        plt.legend(loc=1, bbox_to_anchor=(1.02, 1.28), ncol=2, prop={'size': 11})
        # plt.show()
        plt.savefig("jpg/ETB_" + workload + "_Generic.jpg", bbox_inches='tight', transparent=True)
        plt.savefig("pdf/ETB_" + workload + "_Generic.pdf", bbox_inches='tight', transparent=True)
    plt.close()


@click.command()
@click.option('--purity', default='', show_default=True)
@click.option('--throughput', default='', show_default=True)
@click.option('--cmm', default='', show_default=True)
@click.option('--prog-fct', default='', show_default=True)
@click.option('--prog-kdd', default='', show_default=True)
@click.option('--prog-sensor', default='', show_default=True)
@click.option('--prog-insects', default='', show_default=True)
@click.option('--etb-fct', default='', show_default=True)
@click.option('--etb-kdd', default='', show_default=True)
@click.option('--etb-sensor', default='', show_default=True)
@click.option('--etb-insects', default='', show_default=True)
def draw_all_pictures(purity, throughput, cmm,
                      prog_fct, prog_kdd, prog_sensor, prog_insects,
                      etb_fct, etb_kdd, etb_sensor, etb_insects):
    if purity != '':
        purity_reader = pd.read_csv(purity)
        assert real_world_workloads == [column for column in purity_reader][1:5]
        assert synthetic_workloads == [column for column in purity_reader][5:7]
        assert general_algo_name == purity_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == purity_reader['Algorithm'].to_list()[8:18]
        print('---------start drawing purity of general algorithms on real-world workloads---------')
        purity_and_throughput(purity_reader, True, True, True)
        print('---------start drawing purity of general algorithms on synthetic workloads---------')
        purity_and_throughput(purity_reader, True, False, True)
        print('---------start drawing purity of generic algorithms on real-world workloads---------')
        purity_and_throughput(purity_reader, True, True, False)
        print('---------start drawing purity of generic algorithms on synthetic workloads---------')
        purity_and_throughput(purity_reader, True, False, False)

    if throughput != '':
        throughput_reader = pd.read_csv(throughput)
        assert real_world_workloads == [column for column in throughput_reader][1:5]
        assert synthetic_workloads == [column for column in throughput_reader][5:7]
        assert general_algo_name == throughput_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == throughput_reader['Algorithm'].to_list()[8:18]
        print('---------start drawing throughput of general algorithms on real-world workloads---------')
        purity_and_throughput(throughput_reader, False, True, True)
        print('---------start drawing throughput of general algorithms on synthetic workloads---------')
        purity_and_throughput(throughput_reader, False, False, True)
        print('---------start drawing throughput of generic algorithms on real-world workloads---------')
        purity_and_throughput(throughput_reader, False, True, False)
        print('---------start drawing throughput of generic algorithms on synthetic workloads---------')
        purity_and_throughput(throughput_reader, False, False, False)

    if cmm != '':
        cmm_reader = pd.read_csv(cmm)
        assert general_algo_name == cmm_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == cmm_reader['Algorithm'].to_list()[8:18]
        assert (len(cmm_reader.iloc[i].to_list()) == len(cmm_reader.iloc[i + 1].to_list()) for i in range(18))
        print('---------start drawing CMM of general algorithms on EDS---------')
        cmm(cmm_reader, True)
        print('---------start drawing CMM of general algorithms on EDS---------')
        cmm(cmm_reader, False)

    if prog_fct != '':
        p_fct_reader = pd.read_csv(prog_fct)
        assert general_algo_name == p_fct_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == p_fct_reader['Algorithm'].to_list()[8:18]
        assert (len(p_fct_reader.iloc[i].to_list()) == len(p_fct_reader.iloc[i + 1].to_list()) for i in range(18))
        print('---------start drawing Progressiveness of general algorithms on CoverType---------')
        progressiveness(p_fct_reader, True, 'FCT')
        print('---------start drawing Progressiveness of generic algorithms on CoverType---------')
        progressiveness(p_fct_reader, False, 'FCT')

    if prog_kdd != '':
        p_kdd_reader = pd.read_csv(prog_kdd)
        assert general_algo_name == p_kdd_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == p_kdd_reader['Algorithm'].to_list()[8:18]
        assert (len(p_kdd_reader.iloc[i].to_list()) == len(p_kdd_reader.iloc[i + 1].to_list()) for i in range(18))
        print('---------start drawing Progressiveness of general algorithms on KDD99---------')
        progressiveness(p_kdd_reader, True, 'KDD99')
        print('---------start drawing Progressiveness of generic algorithms on KDD99---------')
        progressiveness(p_kdd_reader, False, 'KDD99')

    if prog_sensor != '':
        p_sensor_reader = pd.read_csv(prog_sensor)
        assert general_algo_name == p_sensor_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == p_sensor_reader['Algorithm'].to_list()[8:18]
        assert (len(p_sensor_reader.iloc[i].to_list()) == len(p_sensor_reader.iloc[i + 1].to_list()) for i in range(18))
        print('---------start drawing Progressiveness of general algorithms on Sensor---------')
        progressiveness(p_sensor_reader, True, 'Sensor')
        print('---------start drawing Progressiveness of generic algorithms on Sensor---------')
        progressiveness(p_sensor_reader, False, 'Sensor')

    if prog_insects != '':
        p_insects_reader = pd.read_csv(prog_insects)
        assert general_algo_name == p_insects_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == p_insects_reader['Algorithm'].to_list()[8:18]
        assert (len(p_insects_reader.iloc[i].to_list()) == len(p_insects_reader.iloc[i + 1].to_list()) for i in
                range(18))
        print('---------start drawing Progressiveness of general algorithms on Insects---------')
        progressiveness(p_insects_reader, True, 'Insects')
        print('---------start drawing Progressiveness of generic algorithms on Insects---------')
        progressiveness(p_insects_reader, False, 'Insects')

    if etb_fct != '':
        etb_fct_reader = pd.read_csv(etb_fct)
        assert general_algo_name == etb_fct_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == etb_fct_reader['Algorithm'].to_list()[8:18]
        assert (len(etb_fct_reader.iloc[i].to_list()) == len(etb_fct_reader.iloc[i + 1].to_list()) for i in
                range(18))
        assert design_aspects == etb_fct_reader.columns.tolist()[1:]
        print('---------start drawing Execution Time Breakdown of general algorithms on CoverType---------')
        execution_time_breakdown(etb_fct_reader, True, 'FCT')
        print('---------start drawing Execution Time Breakdown of generic algorithms on CoverType---------')
        execution_time_breakdown(etb_fct_reader, False, 'FCT')

    if etb_kdd != '':
        etb_kdd_reader = pd.read_csv(etb_kdd)
        assert general_algo_name == etb_kdd_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == etb_kdd_reader['Algorithm'].to_list()[8:18]
        assert (len(etb_kdd_reader.iloc[i].to_list()) == len(etb_kdd_reader.iloc[i + 1].to_list()) for i in
                range(18))
        assert design_aspects == etb_kdd_reader.columns.tolist()[1:]
        print('---------start drawing Execution Time Breakdown of general algorithms on CoverType---------')
        execution_time_breakdown(etb_kdd_reader, True, 'FCT')
        print('---------start drawing Execution Time Breakdown of generic algorithms on CoverType---------')
        execution_time_breakdown(etb_kdd_reader, False, 'FCT')

    if etb_sensor != '':
        etb_sensor_reader = pd.read_csv(etb_sensor)
        assert general_algo_name == etb_sensor_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == etb_sensor_reader['Algorithm'].to_list()[8:18]
        assert (len(etb_sensor_reader.iloc[i].to_list()) == len(etb_sensor_reader.iloc[i + 1].to_list()) for i in
                range(18))
        assert design_aspects == etb_sensor_reader.columns.tolist()[1:]
        print('---------start drawing Execution Time Breakdown of general algorithms on CoverType---------')
        execution_time_breakdown(etb_sensor_reader, True, 'FCT')
        print('---------start drawing Execution Time Breakdown of generic algorithms on CoverType---------')
        execution_time_breakdown(etb_sensor_reader, False, 'FCT')

    if etb_insects != '':
        etb_insects_reader = pd.read_csv(etb_insects)
        assert general_algo_name == etb_insects_reader['Algorithm'].to_list()[0:8]
        assert generic_algo_name == etb_insects_reader['Algorithm'].to_list()[8:18]
        assert (len(etb_insects_reader.iloc[i].to_list()) == len(etb_insects_reader.iloc[i + 1].to_list()) for i in
                range(18))
        assert design_aspects == etb_insects_reader.columns.tolist()[1:]
        print('---------start drawing Execution Time Breakdown of general algorithms on CoverType---------')
        execution_time_breakdown(etb_insects_reader, True, 'FCT')
        print('---------start drawing Execution Time Breakdown of generic algorithms on CoverType---------')
        execution_time_breakdown(etb_insects_reader, False, 'FCT')


if __name__ == '__main__':
    folder = os.getcwd()
    if not os.path.exists(folder + '/jpg'):
        os.makedirs(folder + '/jpg')
    if not os.path.exists(folder + '/pdf'):
        os.makedirs(folder + '/pdf')
    draw_all_pictures()
