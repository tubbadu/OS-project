# Example Application: Audio Tuner

Thanks to the FFT Core module, we wrote an example program capable of recognizing the main tune of a given audio sample. 



## Verification of FFT Results Using MATLAB

To ensure the correctness of the results produced by the module, we used **MATLAB** as a reference tool to compute the Fast Fourier Transform (FFT) of test signals. MATLAB's FFT implementation is highly reliable and widely used in scientific and engineering applications, making it an excellent benchmark for comparison.

## Validation with Lab-created Samples

- **Test Signal Generation**:
    - We generated synthetic test signals in MATLAB, such as sine waves, square waves, or combinations of multiple frequencies.
    - Example of a 440 Hz sine wave generation:
```matlab
Fs = 1500;                    % Sampling frequency (1 kHz)
t = 0:1/Fs:1-1/Fs;            % Time vector (1 second)
f = 440;                      % Frequency of the sine wave (50 Hz)
signal = sin(2 * pi * f * t); % Generate the sine wave
```

- **FFT Computation in MATLAB**:
    - We computed the FFT of the test signal using MATLAB's built-in `fft` function.
    - Example:
```matlab
fft_result_matlab = fft(signal);                                                % Compute the FFT
fft_magnitude_matlab = abs(fft_result_matlab);                                  % Get the magnitude
frequencies = (0:length(fft_result_matlab)-1) * Fs / length(fft_result_matlab); % Frequency axis
```

- **FFT Computation in the Module**:
    - The same test signal was processed by the module to compute the FFT.
    - The module's output was saved for comparison.

- **Comparison of Results**:
    - We compared the FFT results from MATLAB and the module **element by element**.
    - We calculated the **absolute difference** between corresponding frequency bins:
```matlab
difference = abs(fft_magnitude_matlab - fft_magnitude_module);
mse = mean(difference.^2); % Mean Squared Error
fprintf('Mean Squared Error: %f\n', mse);
```
    - A **tolerance threshold** was defined to account for minor numerical differences due to floating-point precision or implementation-specific optimizations.

- **Visualization**:
    - We plotted the FFT results from both MATLAB and the module to visually inspect the agreement.
    - Example:
```matlab
plot(frequencies, fft_magnitude_matlab, 'b', 'LineWidth', 1.5);    % MATLAB FFT
hold on;
plot(frequencies, fft_magnitude_module, 'r--', 'LineWidth', 1.5);  % Module FFT
xlabel('Frequency (Hz)');
ylabel('Magnitude');
legend('MATLAB FFT', 'Module FFT');
title('Comparison of FFT Results');
grid on;
```

- **Validation Metrics**:
    - **Mean Squared Error (MSE)**: Computed to quantify the difference between the two FFT outputs.
    - **Frequency Bin Accuracy**: Ensured that the dominant frequencies identified by the module matched those identified by MATLAB.

- **Edge Case Testing**:
    - We tested the module with various edge cases, such as:
      - Signals with very low or very high amplitudes.
      - Signals with a single frequency component.
      - Signals with added noise to simulate real-world conditions.
    - These tests ensured that the module behaves correctly under a wide range of scenarios.

- **Automated Testing**:
    - We developed a MATLAB script to automate the verification process for multiple test signals.
    - This script generates signals, computes the FFT, compares the results, and logs the metrics.


## Validation with Real-World Samples
We then extended our validation process to **real-world audio samples**, following the same rigorous procedure to ensure the **efficiency** and **accuracy** of our module. This step was crucial to verify the module's performance in practical scenarios, where signals often contain noise, harmonics, and other complexities not present in synthetic test cases.

- **Real-World Signal Acquisition**:
    - We captured real audio samples using a high-quality microphone and preprocessed them to match the module's input requirements.
    - Examples of samples included musical notes played on various instruments, voice recordings, and environmental sounds.

- **FFT Computation and Comparison**:
    - We computed the FFT of these real-world samples using both **MATLAB** (as the reference) and our module.
    - The results were compared using the same metrics as before: **Mean Squared Error (MSE)**, and **frequency bin accuracy**.

- **Observations**:
    - The module demonstrated **high accuracy** for **higher frequencies** (e.g., notes above C1), with results closely matching those from MATLAB.
    - However, we observed a **slightly higher error margin** for **very low frequencies** (e.g., notes in the range C0 to B0). This is likely due to the inherent challenges in accurately resolving low-frequency components with limited sample sizes and the sensitivity of the FFT algorithm at lower frequency ranges.

- **Conclusion on Accuracy**:
    - Despite the minor discrepancies at lower frequencies, the module's performance was deemed **highly accurate** for the majority of the frequency spectrum, particularly in the range most relevant for musical note recognition.
    - The observed errors at lower frequencies are within acceptable limits for our target applications, and we are exploring potential optimizations to further improve accuracy in this range.
    - It is worth noting that such *very low frequencies* (e.g., in the range of C0 to B0, which correspond to frequencies from approximately 16.35 Hz to 30.87 Hz) are **not commonly used in most musical contexts**. In fact, very few real-world instruments are capable of producing frequencies in this range, and even when they do, these notes are rarely utilized in practical musical compositions or performances.
