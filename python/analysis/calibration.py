###
# VBT Trainer Project - Samuel Smith - Spring 2026
# sibsmith@bu.edu | sibsmith.ithaca@gmail.com
###

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 
import sys
import os 

# Paths to data
raw_path = "data/sessions/2026-05-31_19-05_raw.csv"
rep_path = "data/sessions/2026-05-31_19-05_reps.csv"
# FUNCTIONS

def initialize_data(raw_path, rep_path=None):
    """
    Loads raw sample and rep summary CSVs into pandas DataFrames.
 
    Args:
        raw_path : path to the _raw.csv file (required)
        rep_path : path to the _reps.csv file (optional - defaults to none)
 
    Returns:
        (raw_df, rep_df) — rep_df is None if no rep file given
    """

    raw_df = pd.read_csv(raw_path) # Create the raw DataFrame 
    # Create a new time_s columb that is relative time in seconds from the first data point
    raw_df["time_s"] = (raw_df['timestamp_ms'] - raw_df['timestamp_ms'].iloc[0]) / 1000.0 

    rep_df = None
    
    # if a file is requested as an arguement at the path exists, create the rep DataFrame
    if rep_path and os.path.exists(rep_path):
        rep_df = pd.read_csv(rep_path)

    return raw_df, rep_df

def plot_session_velocity(raw_df, rep_df=None):
    """
    Plots smoothed velocity over time for the entire set of reps
    (Optional: overlays rep detection markers)
    """
    fig, ax = plt.subplots(figsize=(14,4))
    # plot the smoothed and raw velocities
    ax.plot(raw_df['time_s'], raw_df['smooth_velocity'], linewidth=0.8, color='steelblue', label='Smoothed velocity')
    ax.plot(raw_df['time_s'], raw_df['raw_velocity'], linewidth=0.4, color='lightgray', alpha=0.6, label='Raw velocity')
    # Create shaded area during each rep time interval
    # GOALS: 
    # - Shade area where each rep took place
    #   - Create a list of lists where 
    # - Highlight peak velocity (with a label)
    # - Create a horizontal line spanning the rep at the height of the MCV
    # if rep_df is not None and len(rep_df) > 0:
        
    # iterates for each rep ('rep_id') and gets the starting and ending times
    for rep_id in sorted(raw_df['rep_num'].unique()): 
        
        if rep_id == 0: # if first rep has not started, continue
            continue
            
        rep_data = raw_df[(raw_df['rep_num'] == rep_id) & (raw_df['raw_velocity'] > 0.05)] # gets all rows of raw_df for that rep_id


        start_time = rep_data['time_s'].min() 
        end_time = rep_data['time_s'].max()
        print(f'{start_time}, {end_time}, on rep: {rep_id}')

        ax.axvspan(start_time, end_time, alpha=0.25)

        
    plt.show()


if __name__ == '__main__':
    # Drop heights are the heights in METERS for each drop
    drop_heights = [0.25, 0.50, 0.75, 1.00]
    # The measured peak velocities of each according to the VBT
    measured_velocities = []

    # _____ LOAD AND PLOT A SESSION MODE
    raw_df, rep_df = initialize_data(raw_path, rep_path)
    plot_session_velocity(raw_df, rep_df)