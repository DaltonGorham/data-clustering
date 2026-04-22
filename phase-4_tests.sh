#!/usr/bin/env bash

# test commands from phase-4. helpful to make sure everything is still working

start=$SECONDS
echo "Running tests at: $(date +"%Y-%m-%d-%T")"
INPUT_FILES=(data/*.txt)
MAX_ITER_ARG=100
THRESHOLD_ARG=0.001
RUNS_ARG=100

for i in "${!INPUT_FILES[@]}"; do
cat << EOF
"Running sample program with input file: ${INPUT_FILES[i]}"
"Max iterations: ${MAX_ITER_ARG}"
"Threshold: ${THRESHOLD_ARG}"
"Runs: ${RUNS_ARG}"
EOF

    ./data_clustering "${INPUT_FILES[i]}" "${MAX_ITER_ARG}" "${THRESHOLD_ARG}" "${RUNS_ARG}"
    echo "---------------------------------------------"
done

echo "All tests completed"
echo "---------------------------------------------"
echo "it took $((SECONDS - start)) seconds"
