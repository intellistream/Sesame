#include "Algorithm/Modular.hpp"
#include <stdio.h>

void parse_args(int argc, char **argv, param_t *cmd_params) {
    int c, i, found;
    /* Flag set by '--verbose'. */
    static int verbose_flag;
    static int nonunique_flag;
    static int fullrange_flag;
    static int basic_numa;
    static int scalarsort = 0;
    static int scalarmerge = 0;
    static int part_fanout = PARTFANOUT_DEFAULT; /* 128 */
    while (1) {
        static struct option long_options[] =
                {
                        /* These options set a flag. */
                        {"verbose",          no_argument,       &verbose_flag,   1},
                        {"brief",            no_argument,       &verbose_flag,   0},
                        {"non-unique",       no_argument,       &nonunique_flag, 1},
                        {"full-range",       no_argument,       &fullrange_flag, 1},
                        {"basic-numa",       no_argument,       &basic_numa,     1},
                        {"help",             no_argument,       0,               'h'},
                        {"version",          no_argument,       0,               'v'},
                        /* These options don't set a flag.
                           We distinguish them by their indices. */
                        {"algo",             required_argument, 0,               'a'},
                        {"nthreads",         required_argument, 0,               'n'},
                        {"perfconf",         required_argument, 0,               'p'},
                        {"r-size",           required_argument, 0,               'r'},
                        {"s-size",           required_argument, 0,               's'},
                        {"perfout",          required_argument, 0,               'o'},
                        {"r-seed",           required_argument, 0,               'x'},
                        {"s-seed",           required_argument, 0,               'y'},
                        {"skew",             required_argument, 0,               'z'},
                        {"r-file",           required_argument, 0,               'R'},
                        {"s-file",           required_argument, 0,               'S'},
                        {"r-key",            required_argument, 0,               'J'},
                        {"s-key",            required_argument, 0,               'K'},
                        {"r-ts",             required_argument, 0,               'L'},
                        {"s-ts",             required_argument, 0,               'M'},
                        /* partitioning fanout, e.g., 2^rdxbits */
                        {"partfanout",       required_argument, 0,               'f'},
                        {"numastrategy",     required_argument, 0,               'N'},
                        {"mwaybufsize",      required_argument, 0,               'm'},
                        {"gen_with_ts",      required_argument, 0,               't'},
                        {"real_data",        required_argument, 0,               'B'},
                        {"window-size",      required_argument, 0,               'w'},
                        {"step-size",        required_argument, 0,               'e'},
                        {"interval",         required_argument, 0,               'l'},
                        {"key_distribution", required_argument, 0,               'd'},
                        {"zipf_param",       required_argument, 0,               'Z'},
                        {"exp_id",           required_argument, 0,               'I'},
                        {"ts_distribution",  required_argument, 0,               'D'},
                        {"duplicate_num",    required_argument, 0,               'P'}, // represent num of Partitions
                        {0, 0,                                  0,               0}
                };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        c = getopt_long(argc, argv, "J:K:L:M:t:w:e:q:l:I:d:Z:D:g:G:B:W:a:n:p:r:s:o:x:y:z:R:S:hv[:]:P:",
                        long_options, &option_index);
        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                MSG("option %s", long_options[option_index].name);
                if (optarg) MSG(" with arg %s", optarg);
                MSG("\n");
                break;
            case 'a':
                i = 0;
                found = 0;
                while (algos[i].joinAlgo) {
                    if (strcmp(optarg, algos[i].name) == 0) {
                        cmd_params->algo = &algos[i];
                        found = 1;
                        break;
                    }
                    i++;
                }
                if (found == 0) {
                    MSG("[ERROR] Join algorithm named `%s' does not exist!\n",
                        optarg);
                    print_help(argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
            case '?':
                /* getopt_long already printed an error message. */
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                print_version();
                exit(EXIT_SUCCESS);
                break;
            case 'n':
                cmd_params->nthreads = atoi(optarg);
                break;
            case 'p':
                cmd_params->perfconf = mystrdup(optarg);
                break;
            case 'r':
                cmd_params->r_size = atol(optarg);
                break;
            case 's':
                cmd_params->s_size = atol(optarg);
                break;
            case 'o':
                cmd_params->perfout = mystrdup(optarg);
                break;
            case 'x':
                cmd_params->r_seed = atoi(optarg);
                break;
            case 'y':
                cmd_params->s_seed = atoi(optarg);
                break;
            case 'z':
                cmd_params->skew = atof(optarg);
                break;
            case 'f':
                part_fanout = atoi(optarg);
                /* check whether fanout is a power of 2 */
                if ((part_fanout & (part_fanout - 1)) != 0) {
                    fprintf(stdout, "[ERROR] Partitioning fan-out must be a power of 2 (2^#radixbits).\n");
                    exit(0);
                }
                break;
            case 'm':
                cmd_params->mwaymerge_bufsize = atoi(optarg);
                break;
            case 'N':
                if (strcmp(optarg, "NEXT") == 0)
                    cmd_params->numastrategy = NEXT;
                else if (strcmp(optarg, "RANDOM") == 0)
                    cmd_params->numastrategy = RANDOM;
                else if (strcmp(optarg, "RING") == 0)
                    cmd_params->numastrategy = RING;
                else {
                    MSG("Invalid NUMA-shuffle strategy. Options: NEXT, RANDOM, RING\n");
                    MSG("Using RING as default.\n");
                }
                break;
            case 'R':
                cmd_params->loadfileR = mystrdup(optarg);
                break;
            case 'S':
                cmd_params->loadfileS = mystrdup(optarg);
                break;
            case 'J':
                cmd_params->rkey = atoi(mystrdup(optarg));
                break;
            case 'K':
                cmd_params->skey = atoi(mystrdup(optarg));
                break;
            case 'L':
                cmd_params->rts = atoi(mystrdup(optarg));
                break;
            case 'M':
                cmd_params->sts = atoi(mystrdup(optarg));
                break;
            case 't':
                cmd_params->ts = atoi(mystrdup(optarg));
                break;
            case 'w':
                cmd_params->window_size = atoi(mystrdup(optarg));
                break;
            case 'e':
                cmd_params->step_sizeR = atoi(mystrdup(optarg));
                break;
            case 'q':
                cmd_params->step_sizeS = atoi(mystrdup(optarg));
                break;
            case 'l':
                cmd_params->interval = atoi(mystrdup(optarg));
                break;
            case 'd':
                cmd_params->key_distribution = atoi(mystrdup(optarg));
                break;
            case 'D':
                cmd_params->ts_distribution = atoi(mystrdup(optarg));
                break;
            case 'Z':
                cmd_params->zipf_param = atof(optarg);
                break;
            case 'I':
                cmd_params->exp_id = atoi(mystrdup(optarg));
                break;
            case 'G':
                cmd_params->group_size = atoi(mystrdup(optarg));
                break;
            case 'g':
                cmd_params->gap = atoi(mystrdup(optarg));
                break;
            case 'B':
                cmd_params->old_param = atoi(mystrdup(optarg));
                break;
            case 'W':
                cmd_params->fixS = atoi(mystrdup(optarg));
                break;
            case '[':
                cmd_params->progressive_step = atoi(mystrdup(optarg));
                break;
            case ']':
                cmd_params->merge_step = atoi(mystrdup(optarg));
                break;
            case 'P':
                cmd_params->duplicate_num = atoi(mystrdup(optarg));
                break;
            default:
                break;
        }
    }
    /* if (verbose_flag) */
    /*     printf ("verbose flag is set \n"); */
    if (cmd_params->step_sizeS == -1) {
        cmd_params->step_sizeS = cmd_params->step_sizeR;
    }
    cmd_params->nonunique_keys = nonunique_flag;
    cmd_params->verbose = verbose_flag;
    cmd_params->fullrange_keys = fullrange_flag;
    cmd_params->basic_numa = basic_numa;
    if (cmd_params->nthreads == 1) {
        cmd_params->group_size = 1;
        MSG("[INFO] reset group size to one\n");
    }
    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        MSG("non-option arguments: ");
        while (optind < argc) MSG("%s ", argv[optind++]);
        MSG("\nc");
    }
}

param_t defaultParam() {/* Command line parameters */
    param_t cmd_params;
    /* Default values if not specified on command line */
    /* BLOCKING HASHING: PRO (0), RJ_st, PRH, PRHO, NPO (4), NPO_st (5),
     * ONLINE HAHSING: SHJ_st(6), SHJ_JM_NP, SHJ_JB_NP, SHJ_JBCR_NP, SHJ_HS_NP (10)
     * ONLINE SORTING: PMJ_st(11), PMJ_JM_NP, PMJ_JB_NP, PMJ_JBCR_NP, PMJ_HS_NP (15)
     * RIPPLE JOIN: RPJ_st(16), RPJ_JM_NP,  RPJ_JB_NP, RPJ_JBCR_NP, RPJ_HS_NP
     * */
    cmd_params.algo = &algos[4];
    cmd_params.nthreads = 1;//TODO: in HS mode, thread must be larger than 1. Fix it when nthread=1.
    cmd_params.r_seed = 12345;
    cmd_params.s_seed = 54321;
    cmd_params.skew = 0.0;
    cmd_params.verbose = 0;
    cmd_params.perfconf = NULL;
    cmd_params.perfout = NULL;
    cmd_params.nonunique_keys = 0;
    cmd_params.fullrange_keys = 0;
    cmd_params.basic_numa = 0;
    cmd_params.scalar_sort = 0;
    cmd_params.scalar_merge = 0;
    /* TODO: get L3 buffer size programmatically. */
    cmd_params.mwaymerge_bufsize = MWAY_MERGE_BUFFER_SIZE_DEFAULT;
    cmd_params.loadfileR = NULL;
    cmd_params.loadfileS = NULL;
    cmd_params.rkey = 0;
    cmd_params.skey = 0;
    cmd_params.old_param = 0;
    cmd_params.window_size = 10000;
    cmd_params.step_sizeR = 40;
    cmd_params.step_sizeS = -1;
    cmd_params.interval = 1000;
    cmd_params.ts = 1;
    cmd_params.key_distribution = 0;
    cmd_params.ts_distribution = 0;
    cmd_params.zipf_param = 0.0;
    cmd_params.exp_id = 0;
    cmd_params.gap = 10;
    cmd_params.group_size = 2;
    cmd_params.fixS = 0;
    /* default dataset is Workload B (described in paper) */
    cmd_params.r_size = 500000;
    cmd_params.s_size = 500000;
    assert(cmd_params.r_size <= cmd_params.s_size);
    return cmd_params;
}

param_t cmd_params = defaultParam();
parse_args(argc, argv, &cmd_params);

int main(int argc, char **argv) {

  Algorithm algo(
      KMeans); // declare a algorithm module, taking kmeans as an example.

  // declare a input benchmark module

  // declare a output evaluate module.
}
