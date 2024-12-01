#!/bin/bash

set -B                  # enable brace expansion
for i in {1..10}; do
  curl localhost:8080
done