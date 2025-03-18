# Kernel Module Documentation: Note Recognition via FFT

## 1. Introduction

This document describes the Linux kernel module developed to perform the **Fast Fourier Transform (FFT)** on an audio signal and recognize the corresponding musical note. The module has been tested in an emulated environment using **QEMU** and **Buildroot**.

---

## 2. Module Objectives

- Capture an input audio signal.
- Apply FFT to obtain the frequency spectrum.
- Identify the dominant frequency and associate it with the corresponding musical note.
- Return the result to the user via kernel logs or a specific interface.

---

## 3. Requirements

- Linux kernel compiled with support for custom modules.
- Buildroot configured to include the necessary libraries.
- Test environment emulated with QEMU.


---

## 4. Implementation

### 4.1 Code Structure

The module is divided into the following main sections:

1. **Initialization**: Registering the module in the kernel and allocating necessary resources.
2. **Signal Acquisition**: Reading the .csv file linked to the audio sample, which contains the recorded data of the sound.
3. **FFT Processing**: Using the Cooley-Tukey FFT method, the module computes the FFT of the input signal.
4. **Note Identification**: The module computes the number of semitones by which the frequency differs from A4 (440 Hz).
5. **Output of Results**: Writing the result to kernel logs.

### 4.2 FFT Functionality

This algorithm implements the **Fast Fourier Transform (FFT)** using an iterative approach optimized for input sizes that are powers of two. Below is a brief description of its key components:

#### Logarithm Base 2 Calculation (`ilog2`)
Computes the integer logarithm base 2 of a given number, used to determine the number of FFT stages.

#### Power of Two Check (`check`)
Verifies if the input size is a power of two, a requirement for the Cooley-Tukey FFT algorithm.

#### Bit-Reversal (`reverse`)
Computes the bit-reversed index of an integer, essential for reordering elements before applying the FFT.

#### Reordering the Input (`ordina`)
Reorders the input vector based on bit-reversed indices to optimize the FFT computation.

#### FFT Computation (`transform`)
- Uses the iterative **Cooley-Tukey FFT** method.
- Initializes a set of complex roots of unity (twiddle factors).
- Applies a series of butterfly operations to iteratively compute the FFT.

#### Final FFT Transformation (`FFT`)
Calls `transform()` and scales the output by the step size.

#### Magnitude Calculation (`modulo`)
Computes the magnitude of a complex number.

#### Sample Count Adjustment (`getNSAMPLES`)
Ensures the number of samples is the next power of two.


This implementation is designed for efficiency and correctness in FFT-based frequency analysis, particularly in audio signal processing.
### 4.3 Frequency-to-Note Mapping

#### Note Recognition (`recog_note`)
- Determines the closest musical note to a given frequency using logarithmic scaling.
- Uses a reference frequency of **440 Hz (A4)** and calculates the closest semitone.
- Maps the computed semitone to musical notes.


---

## 5. Compilation and Installation

To correctly compile and install the module please refer to the **README.md** file 

## 6. Testing on QEMU and Buildroot
# TODO

## 7. Verification of FFT Results Using MATLAB

To ensure the correctness of the results produced by the module, we used **MATLAB** as a reference tool to compute the Fast Fourier Transform (FFT) of test signals. MATLAB's FFT implementation is highly reliable and widely used in scientific and engineering applications, making it an excellent benchmark for comparison.

### Validation with Lab-created Samples

1. **Test Signal Generation**:
   - We generated synthetic test signals in MATLAB, such as sine waves, square waves, or combinations of multiple frequencies.
   - Example of a 440 Hz sine wave generation:
     ```matlab
     Fs = 1500;            % Sampling frequency (1 kHz)
     t = 0:1/Fs:1-1/Fs;    % Time vector (1 second)
     f = 440;               % Frequency of the sine wave (50 Hz)
     signal = sin(2 * pi * f * t);  % Generate the sine wave
     ```

2. **FFT Computation in MATLAB**:
   - We computed the FFT of the test signal using MATLAB's built-in `fft` function.
   - Example:
     ```matlab
     fft_result_matlab = fft(signal);  % Compute the FFT
     fft_magnitude_matlab = abs(fft_result_matlab);  % Get the magnitude
     frequencies = (0:length(fft_result_matlab)-1) * Fs / length(fft_result_matlab);  % Frequency axis
     ```

3. **FFT Computation in the Module**:
   - The same test signal was processed by the module to compute the FFT.
   - The module's output was saved for comparison.

4. **Comparison of Results**:
   - We compared the FFT results from MATLAB and the module **element by element**.
   - We calculated the **absolute difference** between corresponding frequency bins:
     ```matlab
     difference = abs(fft_magnitude_matlab - fft_magnitude_module);
     mse = mean(difference.^2);  % Mean Squared Error
     fprintf('Mean Squared Error: %f\n', mse);
     ```
   - A **tolerance threshold** was defined to account for minor numerical differences due to floating-point precision or implementation-specific optimizations.

5. **Visualization**:
   - We plotted the FFT results from both MATLAB and the module to visually inspect the agreement.
   - Example:
     ```matlab
     plot(frequencies, fft_magnitude_matlab, 'b', 'LineWidth', 1.5);  % MATLAB FFT
     hold on;
     plot(frequencies, fft_magnitude_module, 'r--', 'LineWidth', 1.5);  % Module FFT
     xlabel('Frequency (Hz)');
     ylabel('Magnitude');
     legend('MATLAB FFT', 'Module FFT');
     title('Comparison of FFT Results');
     grid on;
     ```

6. **Validation Metrics**:
   - **Mean Squared Error (MSE)**: Computed to quantify the difference between the two FFT outputs.
   - **Frequency Bin Accuracy**: Ensured that the dominant frequencies identified by the module matched those identified by MATLAB.

7. **Edge Case Testing**:
   - We tested the module with various edge cases, such as:
     - Signals with very low or very high amplitudes.
     - Signals with a single frequency component.
     - Signals with added noise to simulate real-world conditions.
   - These tests ensured that the module behaves correctly under a wide range of scenarios.

8. **Automated Testing**:
   - We developed a MATLAB script to automate the verification process for multiple test signals.
   - This script generates signals, computes the FFT, compares the results, and logs the metrics.


### Validation with Real-World Samples
We then extended our validation process to **real-world audio samples**, following the same rigorous procedure to ensure the **efficiency** and **accuracy** of our module. This step was crucial to verify the module's performance in practical scenarios, where signals often contain noise, harmonics, and other complexities not present in synthetic test cases.

1. **Real-World Signal Acquisition**:
   - We captured real audio samples using a high-quality microphone and preprocessed them to match the module's input requirements.
   - Examples of samples included musical notes played on various instruments, voice recordings, and environmental sounds.

2. **FFT Computation and Comparison**:
   - We computed the FFT of these real-world samples using both **MATLAB** (as the reference) and our module.
   - The results were compared using the same metrics as before: **Mean Squared Error (MSE)**, and **frequency bin accuracy**.

3. **Observations**:
   - The module demonstrated **high accuracy** for **higher frequencies** (e.g., notes above C1), with results closely matching those from MATLAB.
   - However, we observed a **slightly higher error margin** for **very low frequencies** (e.g., notes in the range C0 to B0). This is likely due to the inherent challenges in accurately resolving low-frequency components with limited sample sizes and the sensitivity of the FFT algorithm at lower frequency ranges.

4. **Conclusion on Accuracy**:
   - Despite the minor discrepancies at lower frequencies, the module's performance was deemed **highly accurate** for the majority of the frequency spectrum, particularly in the range most relevant for musical note recognition.
   - The observed errors at lower frequencies are within acceptable limits for our target applications, and we are exploring potential optimizations to further improve accuracy in this range.

### Efficiency Considerations

- The module's **computational efficiency** was consistently validated across all test cases, demonstrating its suitability for **real-time processing**.
- The use of optimized algorithms and careful resource management ensured that the module performed well even with complex real-world signals.

### Future Improvements

- We plan to investigate techniques to enhance the module's accuracy for **very low frequencies**, such as:
  - Increasing the sample size or sampling rate.
  - Applying advanced windowing functions to reduce spectral leakage.
  - Incorporating post-processing algorithms to refine frequency detection.

- It is worth noting that such *very low frequencies* (e.g., in the range of C0 to B0, which correspond to frequencies from approximately 16.35 Hz to 30.87 Hz) are **not commonly used in most musical contexts**. In fact, very few real-world instruments are capable of producing frequencies in this range, and even when they do, these notes are rarely utilized in practical musical compositions or performances.

---

This thorough validation process, using both synthetic and real-world signals, confirms that our module is **reliable** and **accurate** for its intended applications, with only minor limitations at the extreme low end of the frequency spectrum.