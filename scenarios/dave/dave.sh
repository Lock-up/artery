#!/bin/bash

cwd=$(pwd)
sumo_subdir='sumo'
sumo_dir=$sumo_subdir

od2trips_config_file=$sumo_dir/'config/od2trips.config.xml'
duarouter_config_file=$sumo_dir/'config/duarconfig.trips2routes.duarcfg'
sumo_config_file=$sumo_dir/'config/dave_b170.sumocfg'

od2trips -c $od2trips_config_file
duarouter -c $duarouter_config_file
#sumo-gui -c $sumo_config_file