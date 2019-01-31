var data = [0.52, 0.43, 0.4, 0.37, 
  0.43, 0.5, 0.39, 0.38, 0.3, 0.23, 0.3, 0.28, 0.33, 0.31, 
  0.28, 0.25, 0.22, 0.21, 0.19, 0.2, 0.27, 0.28, 0.38, 0.48, 
  0.45, 0.62, 0.69, 0.64, 0.76, 0.87, 0.74, 0.53, 0.56, 0.35, 
  0.34, 0.31, 0.37, 0.37, 0.4, 0.37, 0.47, 0.44, 0.4, 0.38, 
  0.4, 0.38, 0.38, 0.37, 0.44, 0.44, 0.37, 0.33, 0.32, 0.26, 
  0.27, 0.24, 0.23, 0.24, 0.28, 0.3, 0.33, 0.35, 0.38, 0.43, 
  0.46, 0.56, 0.54, 0.47, 0.56, 0.51, 0.52, 0.6, 0.61, 0.84, 
  1.06, 0.97, 1.2, 1.19, 1.22, 1.25, 1.63, 1.71, 1.17, 1.0, 
  1.27, 1.26, 1.31, 1.22, 1.25, 1.26, 1.41, 1.35, 1.22, 1.32, 
  1.18, 1.15, 1.23, 1.16, 1.11, 1.09, 1.2, 1.47, 1.27, 1.22, 
  1.37, 1.37, 1.43, 1.37, 1.09, 1.05, 1.05, 1.25, 1.22, 1.28, 
  1.39, 1.31, 1.15, 0.9, 0.96, 1.15, 1.35, 1.74, 1.79, 2.13, 
  1.72, 1.48, 1.3, 1.45, 1.67, 1.56, 1.62, 1.6, 1.78, 2.04, 
  2.14, 1.84, 1.9, 1.89, 1.52, 1.48, 1.46, 1.43, 1.67, 1.83, 
  1.9, 1.9, 1.69, 1.64, 1.64, 1.81, 1.26, 0.89, 0.85, 0.75, 
  0.99, 1.02, 0.94, 1.06, 1.18, 1.08, 0.97, 0.95, 0.86, 1.09, 
  1.18, 1.1, 1.41, 1.22, 1.28, 1.32, 1.3, 1.16, 1.25, 1.37, 
  1.53, 1.48, 1.42, 1.23, 1.2, 1.26, 1.05, 0.91, 0.91, 0.81, 
  0.81, 0.86, 0.69, 0.73, 0.8, 0.73, 0.76, 0.8, 0.69, 0.55, 
  0.54, 0.6, 0.56, 0.55, 0.47, 0.58, 0.72, 0.72, 0.56, 0.59, 
  0.43, 0.61, 0.78, 0.91, 0.91, 0.88, 0.95, 1.15, 1.03, 1.26, 
  1.23, 1.06, 1.35, 1.36, 1.15, 1.19, 1.52, 1.46, 1.53, 1.84, 
  2.16, 2.09, 2.65, 3.24, 3.4, 3.43, 3.79, 4.49, 4.1, 2.78, 
  3.46, 3.36, 4.03, 1.7, 1.29, 1.09, 0.98, 1.43, 1.21, 1.46, 
  1.69, 1.32, 1.54, 1.24, 1.23, 1.03, 1.16, 1.41, 1.45, 1.64, 
  1.44, 1.57, 1.61, 1.54, 1.17, 1.01, 0.98, 0.96, 1.06, 1.03, 
  0.95, 0.95, 0.99, 0.94, 0.94, 1.19, 1.26, 1.39, 1.5, 1.37, 
  1.51, 1.38, 1.41, 1.49, 1.58, 1.79, 1.71, 1.86, 2.15, 2.14, 
  2.28, 2.56, 3.47, 4.44, 4.26, 5.09, 5.93, 5.51, 4.77, 5.26, 
  4.87, 5.64, 6.2, 7.09, 7.62, 8.97, 9.51, 9.99, 9.06, 8.3, 
  9.31, 7.91, 7.58, 8.99, 8.98, 10.18, 10.73, 12.13, 11.22, 11.34, 
  11.19, 12.29, 13.2, 16.03, 16.15, 17.43, 18.32, 20.3, 25.13, 24.11, 
  26.21, 17.91, 16.54, 18.98, 23.01, 24.97, 22.15, 21.03, 22.43, 15.04, 
  14.23, 12.26, 11.29, 11.92, 11.82, 13.91, 16.65, 17.97, 18.84, 21.62, 
  22.25, 24.52, 24.94, 26.45, 27.88, 25.41, 27.07, 31.09, 34.54, 33.98, 
  33.28, 34.03, 32.16, 37.54, 39.82, 41.16, 42.67, 44.89, 46.73, 46.11, 
  46.32, 46.02, 44.41, 51.66, 50.91, 50.45, 53.55, 50.56, 53.58, 60.39, 
  71.76, 79.32, 77.26, 76.43, 77.26, 80.8, 88.39, 88.64, 79.1, 78.12, 
  71.03, 60.8, 59.26, 59.43, 59.44, 60.78, 53.59, 61.15, 66.28, 64.85, 
  71.1, 76.09, 76.76, 68.5, 72.43, 73.88, 81.22, 87.62, 90.04, 92.63, 
  99.81, 98.1, 105.16, 116.31, 107.95, 114.58, 126.12, 122.17, 122.87, 128.44, 
  123.66, 119.59, 111.67, 109.24, 118.35, 117.66, 104.69, 96.81, 96.69, 102.26];