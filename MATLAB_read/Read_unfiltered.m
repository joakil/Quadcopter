%% Close all ports before starting (Also before uploading arduino data)
clc;
clear all;
allObj = instrfind;
if (~isempty(allObj))
    fclose(allObj);
    delete(allObj);
end

%%
clc;
clear all;

COM_PORT = serial('COM7', 'DataBits', 8, 'StopBits', 1, 'BaudRate', 9600, 'Parity', 'none');
fopen(COM_PORT);

tic;
fprintf('Trying to connect with Arduin...\n')
from_Arduino = 'a';
while from_Arduino ~= 's'
    elapsed_time = toc;
    if COM_PORT.BytesAvailable > 0
        from_Arduino = fread(COM_PORT,1, 'uchar');     %fread(COM5,1,'int');
        fprintf(COM_PORT, '%c', 's');
    elseif elapsed_time > 10
        error('Failed to connect :( Restart Arduino and try again');
    end
end
fprintf('Connected! Receiving data series...\n')


i = 1;
empty = fscanf(COM_PORT, '%f'); % First read is empty (not sure why)

tic
while elapsed_time < 50
    elapsed_time = toc;
    
    euler(1,i) = fscanf(COM_PORT, '%u')/1000000; %from microseconds to seconds
    euler(2,i) = fscanf(COM_PORT, '%f')/1000; %Filtered Pitch
    euler(3,i) = fscanf(COM_PORT, '%f')/1000; %Filtered Roll
    euler(4,i) = fscanf(COM_PORT, '%f')/1000; %Filtered Yaw
    euler(5,i) = fscanf(COM_PORT, '%f')/1000; % Pitch
    euler(6,i) = fscanf(COM_PORT, '%f')/1000; % Roll
    euler(7,i) = fscanf(COM_PORT, '%f')/1000; % Pitch velocity
    euler(8,i) = fscanf(COM_PORT, '%f')/1000; % Roll velocity
    euler(9,i) = fscanf(COM_PORT, '%f')/1000; % Yaw velocity
    
    fprintf('Data Received \n')
    
    i = i+1;
end

fclose(COM_PORT);
delete(COM_PORT);


% plot(euler(1,:), euler(2,:),euler(1,:), euler(3,:),euler(1,:), euler(4,:))

%% UNLY USED FOR SOME TESTING

COM_PORT = serial('COM7', 'DataBits', 8, 'StopBits', 1, 'BaudRate', 9600, 'Parity', 'none');
fopen(COM_PORT);
asd = 'n';
while true
    asd = fread(COM_PORT,1, 'uchar');
    fprintf('%c', asd)
    
    if asd == 'a'
        fprintf(COM_PORT, '%c', 's');
    end
    
end






