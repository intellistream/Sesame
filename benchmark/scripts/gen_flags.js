flags = ["algo", "input_file", "num_points", "dim", "num_clusters", "max_in_nodes", "max_leaf_nodes", "distance_threshold", "seed", "coreset_size", "radius", "delta", "beta", "buf_size", "alpha", "lambda", "clean_interval", "min_weight", "base", "cm", "cl", "grid_width", "min_points", "epsilon", "mu", "num_last_arr", "time_window", "num_online_clusters", "delta_grid", "num_samples"];

// console.log(flags.length)

// flags.map(x => { console.log(`cmd_params.${x} = FLAGS_${x};`) });

// flags.map(x => { console.log(`std::cerr << "${x}: " << ${x} << std::endl;`) });

// flags.map(x => { console.log(`${x} = `) });

// flags.map(x => { console.log(`bench.sesame.${x} = ${x}`) });

// flags.map(x => { console.log(`${x}=\`must_env_val "\${env}" 'bench.sesame.${x}'\``) });

// flags.map(x => { console.log(`--${x}="\${${x}}" \\`) });

// flags.map(x => { console.log(`\t\t${x} DOUBLE,\\`) });

flags.map(x => { process.stdout.write(` ${x}=[[bench.sesame.${x}]]`) });
