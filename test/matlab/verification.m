clc;
clear all;
close all;


Fs = 1470;            % Sampling frequency (1 kHz)
t = 0:1/Fs:1-1/Fs;    % Time vector (1 second)
f = 440;               % Frequency of the sine wave (50 Hz)

signal_mat=csvread("Eb_311Hz.csv");      % Import the generated signal
near_power=ceil(log2(size(signal_mat,1)));
padded_signal=zeros(2^(near_power),1);
for i=1:size(signal_mat,1)
	padded_signal(i)=signal_mat(i,1);
end 
fft_c= fftshift(csvread("output.csv"));

fft_result_matlab = fft(padded_signal);  % Compute the FFT
fft_magnitude_matlab = fftshift(abs(fft_result_matlab));  % Get the magnitude
L=length(fft_result_matlab);   
frequencies = Fs/L*(-L/2:L/2-1); % Frequency axis

difference = abs(fft_magnitude_matlab - fft_c);
mse = mean(difference.^2);  % Mean Squared Error
fprintf('Mean Squared Error: %f\n', mse);

plot(frequencies, fft_magnitude_matlab, 'b', 'LineWidth', 1.5);  % MATLAB FFT
hold on;
plot(frequencies, fft_c, 'r', 'LineWidth', 1.5);  % Module FFT
xlabel('Frequency (Hz)');
ylabel('Magnitude');
legend('MATLAB FFT', 'Module FFT');
title('Comparison of FFT Results');
grid on;
