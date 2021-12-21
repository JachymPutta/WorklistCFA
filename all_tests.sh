#!/bin/bash
for D in tests/*/ ; do
    echo "Executing: $D";
    ./main "$D";
    echo "$D done.";
done

