#!/usr/bin/env bash

# test commands from phase-2. helpful to make sure everything is still working


INPUT_FILES=(data/*.txt)  
CLUSTERS_ARGS=(8 6 2 3 6 26 7 4 3 10)
MAX_ITER_ARG=100
THRESHOLD_ARG=0.001
RUNS_ARG=100

for i in "${!INPUT_FILES[@]}"; do
cat << EOF
"Running sample program with input file: ${INPUT_FILES[i]}"
"Clusters argument: ${CLUSTERS_ARGS[i]}"
"Max iterations: ${MAX_ITER_ARG}"
"Threshold: ${THRESHOLD_ARG}"
"Runs: ${RUNS_ARG}"
EOF

    ./data_clustering "${INPUT_FILES[i]}" "${CLUSTERS_ARGS[i]}" "${MAX_ITER_ARG}" "${THRESHOLD_ARG}" "${RUNS_ARG}"
    echo "---------------------------------------------"
done
