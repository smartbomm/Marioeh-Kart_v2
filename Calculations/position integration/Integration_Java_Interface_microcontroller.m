%% Start program
clc;
close all;
clear all;
% Define variables
source_data = "Section_5_C_Code_Buffersize30_debug_integrationonmc.xlsx";
%Scaling 
Scaler_accel = 4096;
Scaler_gyro = 512;
Buffersize = 34;
%% Read data from Excel into Workspace
data_time = table2array(readtable(source_data,'Sheet',1,TextType='string', VariableNamingRule= 'preserve'));
data_x = table2array(readtable(source_data,'Sheet',2,TextType='string', VariableNamingRule= 'preserve'));
data_y = table2array(readtable(source_data,'Sheet',3,TextType='string', VariableNamingRule= 'preserve'));
data_z = table2array(readtable(source_data,'Sheet',4,TextType='string', VariableNamingRule= 'preserve'));
gyro_data_x = table2array(readtable(source_data,'Sheet',5,TextType='string', VariableNamingRule= 'preserve'));
gyro_data_y = table2array(readtable(source_data,'Sheet',6,TextType='string', VariableNamingRule= 'preserve'));
gyro_data_z = table2array(readtable(source_data,'Sheet',7,TextType='string', VariableNamingRule= 'preserve'));
data_accel_L = table2array(readtable(source_data,'Sheet',8,TextType='string', VariableNamingRule= 'preserve'));
data_speed_L = table2array(readtable(source_data,'Sheet',9,TextType='string', VariableNamingRule= 'preserve'));
data_pos_L = table2array(readtable(source_data,'Sheet',10,TextType='string', VariableNamingRule= 'preserve'));
data_track_s = table2array(readtable(source_data,'Sheet',11,TextType='string', VariableNamingRule= 'preserve'));
data_error = table2array(readtable(source_data,'Sheet',12,TextType='string', VariableNamingRule= 'preserve'));
%% Pre-Processing
height_vector = [height(data_x),height(data_y),height(data_z),height(gyro_data_x),height(gyro_data_y),height(gyro_data_z),height(data_accel_L),height(data_speed_L),height(data_pos_L),height(data_time),height(data_track_s),height(data_error)];
target_length=min(height_vector); %kürzen auf eine Länge
data_x=data_x(1:target_length,:);
data_y=data_y(1:target_length,:);
data_z=data_z(1:target_length,:);
gyro_data_x=gyro_data_x(1:target_length,:);
gyro_data_y=gyro_data_y(1:target_length,:);
gyro_data_z=gyro_data_z(1:target_length,:);
data_time=data_time(1:target_length,:);
data_accel_L=data_accel_L(1:target_length,:);
data_speed_L=data_speed_L(1:target_length,:);
data_pos_L=data_pos_L(1:target_length,:);
data_track_s=data_track_s(1:target_length,:);
data_error=data_error(1:target_length,:);
%% Convert all data
% Conversion to double 
acc_x = str2double(data_x);
acc_y = str2double(data_y);
acc_z = str2double(data_z);
gyro_x = str2double(gyro_data_x);
gyro_y = str2double(gyro_data_y);
gyro_z = str2double(gyro_data_z);
time = str2double(data_time);
acc_L = str2double(data_accel_L);
speed_L = str2double(data_speed_L);
pos_L = str2double(data_pos_L);
track_s = str2double(data_track_s);
err = str2double(data_error);
% Convert to readable format
acc_x = (9.81*acc_x)/(Scaler_accel);
%acc_y = (9.81*acc_y)/(Scaler_accel);
acc_z = (9.81*acc_z)/(Scaler_accel*Buffersize);
gyro_x =(9.81*gyro_x)/(Scaler_accel*Buffersize);
gyro_y = (9.81*gyro_y)/(Scaler_accel*Buffersize); % Calculated complete acceleration
gyro_z = (gyro_z)/(1000);
acc_L = (9.81*acc_L)/(Scaler_accel*Buffersize);
speed_L = speed_L/(1000); %von mircometers/millisecond to m/s
pos_L = pos_L/1000;    %von micrometers to meters
%% Plot unfiltered Data
figure Name "Data";
tile = tiledlayout(4,3);
nexttile;
plot(acc_x,"Color","r");
title("UNFILTERED Acceleration X-Axis [m/s]");

nexttile;
plot(acc_y,"Color","r");
title("UNFILTERED Acceleration Y-Axis [m/s]");

nexttile;
plot(acc_z,"Color","r");
title("UNFILTERED Acceleration Z-Axis [m/s]");

nexttile;
plot(gyro_x,"Color","r");
title("Merker  Beschleunigung X");

nexttile;
plot(gyro_y,"Color","r");
title("Gyro Y");

nexttile;
plot(gyro_z,"Color","r");
title("Barcode Reader Velocity");

nexttile;
plot(err,"Color","r");
title("Error Barcode Reader");



%% Plot filtered Data
nexttile;
plot(acc_L,"Color","b");
title("FILTERED Acceleration X-Axis [m/s]");

nexttile;
plot(speed_L,"Color","g");
title("Speed Linear")

nexttile;
plot(pos_L,"Color","r");
title("Position");

nexttile;
plot(track_s,"Color","b");
title("Track Section");




