
// ******** Santa Clara County population, 1860 ~ 2010 ********************
var sc_population = {};
sc_population.y = [11912, 26246, 35039, 48005, 60216, 83539,
              100676, 145118, 174949, 290547, 642315, 1065313,
              1295071, 1497577, 1682585, 1781642, ];
sc_population.x = [];
for (var i = 1860; i <= 2010; i = i + 10)
  sc_population.x.push(i);

// USA population, 1860 ~ 2010
var usa_population = {};
usa_population.y = [31443321, 39818449, 50189209, 62947714, 76212168, 92228496,
                    106021537, 122775046, 132164569, 150697361, 179323175,
                    203302031, 226545805, 248709873, 281421906, 308745538];
usa_population.x = sc_population.x;


// ********** Population of other counties in the Bay Area, 1860 ~ 2010 ****
var alameda_population = [8927, 24237, 62976, 93864, 130197, 246131, 344177,
    474883, 513011, 740315, 908209, 1071446, 1105379, 1279182, 1443741, 1510271];

var contra_costa_population = [5328, 8461, 12525, 13515, 18046, 31674, 53889,
    78608, 100450, 298984, 409030, 556116, 656380, 803732, 948816, 1049025];

var marin_population = [3334, 6903, 11324, 13072, 15702, 25114, 27342, 41648,
    52907, 85619, 146820, 208652, 222568, 230096, 247289, 252409];

var napa_population = [5521, 7163,  13235, 16411, 16451, 19800, 20678, 22897,
    28503, 46603, 65890, 79140, 99199, 110765, 124279, 136484];

var sf_population = [56802, 149473, 233959, 298997, 342782, 416912, 506676,
    634394, 634536, 775357, 740316, 715674, 678974, 723959, 776733, 808235];

var san_mateo_population = [3214, 6635, 8669, 10087, 12094, 26585, 36781, 77405,
    111782, 235659, 444387, 557361, 587329, 649623, 707161, 718451];

var solano_population = [7169, 16871, 18475, 20946, 24143, 27559, 40602, 40834,
    49118, 104833, 134597, 171989, 235203, 340421, 394542, 413344];

var sonoma_population = [11867, 19819, 25926, 32721, 38480, 48394, 52090, 62222,
    69052, 103405, 147375, 204885, 299681, 388222, 458614, 483878];



// ************* Ethnic Distribution Data, 1950 ~ 2010 *******************

var sc_white_population = [280429, 621625, 1003898, 1146138, 1184583,
                           1147523, 1106400];
var sc_asian_population = [6815, 15864, 35237, 95835, 251593, 432424,
                           571907];
var sc_black_population = [1718, 4187, 18090, 41442, 52415, 43747,
                           42759];
var sc_other_population = [1585, 639, 7489, 11656, 8985, 58890, 60576];

// generate the 1950 ~ 2010 years
var ethnic_data_years = [];
for (var i = 1950; i <= 2010; i += 10)
  ethnic_data_years.push(i);

// USA Asian percentage, 1950 ~ 2010
var usa_asian_percentage = [0.002130316, 0.005466873, 0.00751144, 0.015451352,
                            0.02924557, 0.037814515, 0.049277684];

// ********* Stock Market Data ***********************************
var aapl_stock = {},
    csco_stock = {},
    goog_stock = {},
    hpq_stock = {},
    intc_stock = {};

aapl_stock.y = [0.52, 0.43, 0.4, 0.37,
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

csco_stock.y = [0.07, 0.08,
  0.08, 0.08, 0.08, 0.07, 0.09, 0.12, 0.14, 0.18, 0.17, 0.15,
  0.18, 0.2, 0.19, 0.24, 0.28, 0.25, 0.3, 0.3, 0.4, 0.46,
  0.5, 0.48, 0.45, 0.56, 0.57, 0.65, 0.57, 0.65, 0.73, 0.91,
  0.96, 1.08, 1.07, 1.09, 1.01, 1.32, 1.34, 1.27, 1.17, 1.22,
  1.25, 1.37, 1.58, 1.77, 1.8, 1.67, 1.48, 1.21, 1.14, 1.03,
  1.21, 1.34, 1.47, 1.57, 1.71, 1.63, 1.65, 1.86, 1.95, 2.14,
  2.47, 2.72, 3.2, 3.37, 3.78, 4.11, 3.64, 4.06, 4.64, 4.53,
  5.06, 5.35, 5.53, 5.05, 5.15, 6.06, 6.04, 6.63, 6.21, 6.81,
  5.43, 4.7, 5.05, 6.61, 6.55, 7.77, 7.36, 7.13, 8.01, 8.42,
  8.16, 9.23, 9.65, 10.01, 10.73, 11.07, 13.48, 14.02, 11.99, 13.58,
  13.84, 16.56, 20.39, 24.51, 21.49, 24.07, 25.05, 23.94, 28.31, 27.29,
  29.79, 30.12, 32.51, 39.18, 47.06, 48.11, 58.07, 67.93, 60.91, 50.03,
  55.85, 57.5, 60.3, 48.54, 47.34, 42.06, 33.61, 32.89, 20.81, 13.89,
  14.92, 16.92, 15.99, 16.89, 14.35, 10.7, 14.87, 17.96, 15.91, 17.4,
  12.54, 14.88, 12.87, 13.86, 12.26, 11.59, 12.14, 9.21, 9.82, 13.11,
  11.51, 11.75, 12.28, 11.4, 13.18, 14.42, 14.75, 17.12, 16.82, 17.21,
  18.39, 19.95, 21.29, 22.59, 20.35, 20.71, 18.37, 19.66, 20.82, 18.38,
  16.48, 15.9, 16.88, 16.47, 16.98, 15.85, 15.31, 15.72, 15.17, 17.05,
  16.76, 16.83, 15.48, 15.75, 15.33, 15.41, 15.04, 16.32, 17.78, 19.04,
  18.41, 17.29, 17.16, 15.71, 19.32, 20.19, 21.2, 23.64, 24.01, 23.39,
  22.79, 22.43, 23.49, 23.65, 24.47, 25.4, 28.05, 29.11, 29.05, 24.62,
  23.78, 21.53, 21.43, 21.17, 22.53, 23.48, 20.44, 19.32, 21.13, 19.82,
  15.61, 14.53, 14.32, 13.15, 12.8, 14.73, 16.98, 16.25, 16.39, 19.34,
  18.98, 20.68, 20.04, 20.56, 21.03, 19.74, 21.38, 22.87, 23.66, 20.35,
  18.72, 20.27, 17.56, 19.24, 20.09, 16.83, 17.77, 18.58, 16.31, 15.12,
  15.45, 14.81, 13.76, 14.13, 13.87, 13.72, 16.47, 16.56, 16.07, 17.52,
  17.72, 18.86, 18.04, 14.61, 15.37, 14.34, 17.15, 17.17, 15.53, 17.25,
  17.93, 18.77, 19.03, 19.07, 19.24, 22.19, 22.39, 23.7, 21.59, 21.7,
  21.05, 19.83, 20.93, 20.6, 20.5, 21.08, 21.91, 23.35, 23.56, 24.11,
  23.88, 24.23, 23.56, 26.61, 26.78, 25.56, 28.61, 26.89, 28.16, 28.63,
  26.82, 27.97, 25.47, 25.84, 28.63, 27.04, 26.95, 23.79, 26.18, 27.86];

goog_stock.y = [64.74, 95.22, 90.9, 96.3, 97.71, 93.9, 90.16, 109.89, 138.5,
  146.93, 143.74, 142.86, 158.07, 185.88, 202.25, 207.22, 216.11, 181.13, 194.81,
  208.76, 185.72, 209.46, 193.11, 189.08, 200.75, 237.96, 242.16, 230.01, 250.5,
  224.5, 228.85, 235.45, 248.71, 261.09, 254.75, 257.37, 283.35, 353.15, 346.15,
  345.4, 281.87, 235.36, 220.02, 286.86, 292.61, 262.95, 236.64, 231.41, 200.06,
  179.5, 146.33, 153.67, 169.1, 168.83, 173.86, 197.79, 208.41, 210.58, 221.3,
  230.6, 247.68, 267.79, 291.21, 309.68, 264.71, 263.14, 283.28, 262.59, 242.57,
  222.25, 242.18, 224.79, 262.63, 306.54, 277.58, 296.69, 299.88, 306.39, 293.09,
  271.78, 264.25, 252.94, 301.54, 270.21, 257.26, 296.02, 299.4, 322.63, 289.77,
  308.82, 320.3, 302.12, 290.14, 289.75, 316.17, 342.2, 376.87, 339.81, 348.84,
  353.34, 377.47, 400.2, 396.7, 411.87, 435.18, 439.75, 443.43, 423.03, 437.52,
  514.78, 529.27, 559.8, 589.9, 607.22, 556.97, 526.66, 559.89, 575.28, 571.6,
  571.6, 577.36, 559.08, 541.83, 526.4, 534.52, 558.4, 548.0, 537.34, 532.11,
  520.51, 625.61, 618.25, 608.42, 710.81, 742.6, 758.88, 742.95, 697.77, 726.82];

hpq_stock.y = [0.05, 0.05, 0.05, 0.04, 0.04, 0.03, 0.03, 0.04, 0.03, 0.03, 0.04, 
  0.04, 0.04, 0.03, 0.03, 0.04, 0.03, 0.03, 0.03, 0.03, 0.03, 
  0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 0.03, 
  0.03, 0.03, 0.03, 0.03, 0.04, 0.04, 0.04, 0.04, 0.04, 0.04, 
  0.04, 0.05, 0.05, 0.06, 0.05, 0.06, 0.06, 0.06, 0.07, 0.07, 
  0.08, 0.07, 0.07, 0.08, 0.07, 0.07, 0.07, 0.08, 0.08, 0.09, 
  0.09, 0.1, 0.12, 0.11, 0.12, 0.13, 0.12, 0.13, 0.13, 0.13, 
  0.11, 0.1, 0.09, 0.12, 0.13, 0.13, 0.13, 0.12, 0.12, 0.12, 
  0.13, 0.14, 0.13, 0.13, 0.12, 0.14, 0.14, 0.14, 0.13, 0.14, 
  0.14, 0.15, 0.17, 0.16, 0.16, 0.16, 0.16, 0.14, 0.13, 0.09, 
  0.07, 0.07, 0.08, 0.09, 0.08, 0.08, 0.1, 0.11, 0.11, 0.13, 
  0.13, 0.13, 0.13, 0.12, 0.12, 0.14, 0.13, 0.13, 0.15, 0.16, 
  0.18, 0.18, 0.18, 0.22, 0.22, 0.23, 0.23, 0.23, 0.25, 0.27, 
  0.31, 0.32, 0.31, 0.3, 0.29, 0.29, 0.28, 0.31, 0.29, 0.31, 
  0.35, 0.28, 0.29, 0.3, 0.29, 0.31, 0.3, 0.32, 0.3, 0.26, 
  0.26, 0.21, 0.24, 0.21, 0.22, 0.24, 0.3, 0.33, 0.38, 0.41, 
  0.43, 0.39, 0.34, 0.35, 0.36, 0.35, 0.34, 0.39, 0.39, 0.42, 
  0.38, 0.38, 0.42, 0.39, 0.32, 0.34, 0.31, 0.34, 0.32, 0.28, 
  0.26, 0.27, 0.25, 0.27, 0.29, 0.29, 0.29, 0.28, 0.26, 0.27, 
  0.27, 0.24, 0.23, 0.23, 0.28, 0.28, 0.3, 0.32, 0.32, 0.32, 
  0.28, 0.31, 0.33, 0.32, 0.31, 0.34, 0.35, 0.34, 0.36, 0.35, 
  0.42, 0.43, 0.39, 0.42, 0.44, 0.5, 0.48, 0.47, 0.43, 0.45, 
  0.46, 0.56, 0.53, 0.56, 0.55, 0.67, 0.67, 0.61, 0.65, 0.69, 
  0.73, 0.79, 0.71, 0.71, 0.65, 0.63, 0.68, 0.62, 0.59, 0.67, 
  0.63, 0.6, 0.66, 0.65, 0.64, 0.62, 0.81, 0.79, 0.93, 1.09, 
  1.1, 1.19, 1.26, 1.2, 1.13, 1.25, 1.37, 1.28, 1.31, 1.25, 
  1.06, 1.2, 1.28, 1.21, 1.07, 1.11, 1.12, 1.06, 1.1, 1.1, 
  1.19, 1.13, 1.1, 0.99, 1.03, 1.14, 1.12, 1.03, 0.96, 1.01, 
  1.06, 1.15, 1.08, 0.99, 0.92, 1.09, 1.12, 1.2, 1.34, 1.34, 
  1.38, 1.41, 1.25, 1.2, 1.42, 1.18, 1.19, 1.33, 1.28, 1.54, 
  1.68, 1.74, 1.72, 2.0, 1.88, 1.88, 2.03, 2.17, 1.54, 1.4, 
  1.79, 1.75, 1.9, 1.87, 1.88, 1.74, 1.72, 1.56, 1.43, 1.54, 
  1.56, 1.49, 1.65, 1.79, 1.71, 1.59, 1.76, 1.67, 1.61, 1.67, 
  1.63, 1.56, 1.48, 1.41, 1.47, 1.39, 1.34, 1.43, 1.36, 1.49, 
  1.48, 1.35, 1.09, 1.05, 0.82, 0.94, 1.0, 1.23, 1.47, 1.58, 
  1.62, 1.71, 1.61, 1.7, 1.67, 1.57, 1.6, 1.53, 1.81, 1.89, 
  2.32, 2.57, 2.55, 2.43, 2.2, 2.35, 1.83, 1.79, 1.83, 2.14, 
  2.25, 2.32, 2.38, 2.46, 2.42, 2.69, 2.63, 2.33, 2.4, 2.22, 
  2.39, 2.4, 2.58, 2.78, 2.96, 2.69, 2.63, 2.57, 2.47, 2.55, 
  2.96, 2.88, 3.23, 3.22, 3.3, 3.32, 3.8, 3.99, 4.38, 4.38, 
  4.95, 5.18, 5.32, 5.56, 6.17, 5.53, 5.6, 5.66, 6.73, 6.3, 
  7.09, 7.13, 6.69, 5.91, 5.87, 6.56, 5.94, 7.25, 6.78, 7.1, 
  7.57, 7.2, 7.1, 6.96, 7.59, 9.49, 8.33, 9.45, 8.37, 8.3, 
  8.49, 8.18, 9.12, 8.64, 10.28, 8.5, 8.19, 7.59, 6.64, 7.26, 
  8.27, 8.59, 9.4, 10.78, 9.14, 9.35, 10.87, 13.0, 13.88, 14.46, 
  14.55, 12.55, 10.26, 13.12, 15.76, 15.0, 18.63, 18.39, 18.72, 16.67, 
  22.22, 19.44, 21.47, 17.29, 16.57, 11.27, 11.28, 13.17, 10.31, 11.21, 
  10.19, 10.51, 10.28, 8.86, 8.34, 5.79, 6.08, 7.94, 7.44, 8.01, 
  7.29, 6.53, 6.22, 6.95, 5.59, 5.17, 4.91, 4.29, 5.81, 7.16, 
  6.41, 6.43, 5.85, 5.77, 6.05, 7.24, 7.94, 7.89, 7.43, 7.24, 
  8.35, 8.13, 8.62, 8.93, 8.53, 8.61, 7.42, 8.0, 7.98, 7.62, 
  6.77, 7.12, 7.09, 7.6, 8.0, 7.47, 7.93, 8.4, 7.84, 8.62, 
  9.03, 9.46, 10.66, 11.25, 10.8, 11.43, 11.06, 12.04, 12.67, 12.74, 
  12.57, 12.54, 12.3, 12.39, 14.2, 14.28, 15.08, 15.36, 16.06, 16.88, 
  15.34, 15.68, 16.46, 17.86, 17.46, 18.02, 19.31, 19.52, 20.26, 20.06, 
  19.82, 17.17, 18.76, 17.96, 18.23, 18.51, 17.42, 17.65, 18.48, 18.25, 
  15.11, 13.92, 14.35, 13.75, 11.48, 12.72, 14.27, 13.63, 15.37, 17.21, 
  17.85, 18.8, 18.9, 19.54, 20.55, 18.78, 20.26, 21.23, 20.76, 18.38, 
  17.32, 18.42, 15.39, 16.87, 16.86, 16.82, 16.92, 18.36, 17.53, 16.49, 
  16.25, 15.05, 14.7, 14.21, 10.51, 9.12, 10.81, 11.35, 10.51, 11.41, 
  10.32, 9.77, 10.15, 9.3, 8.29, 7.52, 6.96, 7.09, 5.75, 5.4, 
  5.98, 6.92, 8.45, 10.06, 8.69, 10.31, 10.53, 10.9, 9.48, 8.97, 
  10.41, 11.69, 12.02, 12.46, 12.84, 13.97, 14.27, 14.46, 14.61, 15.44, 
  16.48, 15.45, 15.63, 17.01, 17.55, 15.8, 15.24, 13.69, 14.49, 14.68, 
  13.26, 13.48, 12.4, 11.39, 11.99, 12.28, 11.71, 9.6, 10.57, 11.7];

intc_stock.y = [0.22, 0.25,
  0.24, 0.31, 0.32, 0.32, 0.31, 0.34, 0.29, 0.27, 0.25, 0.26,
  0.28, 0.3, 0.26, 0.25, 0.2, 0.18, 0.21, 0.18, 0.16, 0.19,
  0.19, 0.21, 0.23, 0.22, 0.23, 0.23, 0.27, 0.24, 0.27, 0.28,
  0.28, 0.31, 0.34, 0.35, 0.36, 0.42, 0.54, 0.59, 0.6, 0.6,
  0.55, 0.61, 0.61, 0.54, 0.54, 0.52, 0.51, 0.45, 0.44, 0.44,
  0.52, 0.45, 0.4, 0.42, 0.41, 0.45, 0.41, 0.4, 0.4, 0.38,
  0.38, 0.41, 0.38, 0.38, 0.37, 0.42, 0.43, 0.41, 0.4, 0.38,
  0.42, 0.4, 0.34, 0.27, 0.34, 0.28, 0.3, 0.34, 0.31, 0.46,
  0.55, 0.57, 0.68, 0.64, 0.64, 0.7, 0.77, 0.84, 0.57, 0.47,
  0.58, 0.56, 0.62, 0.64, 0.68, 0.67, 0.79, 0.76, 0.61, 0.6,
  0.54, 0.44, 0.52, 0.57, 0.56, 0.57, 0.63, 0.71, 0.63, 0.67,
  0.66, 0.7, 0.74, 0.76, 0.75, 0.86, 0.88, 0.91, 0.86, 1.05,
  1.03, 0.97, 0.73, 0.73, 0.73, 0.82, 0.84, 1.0, 1.04, 1.02,
  1.08, 1.22, 1.02, 1.03, 1.08, 0.92, 0.89, 0.9, 1.07, 1.31,
  1.39, 1.21, 1.17, 1.09, 1.25, 1.31, 1.27, 1.44, 1.48, 1.56,
  1.9, 2.34, 2.55, 2.52, 2.09, 2.43, 2.41, 2.29, 2.82, 3.11,
  2.78, 2.7, 2.72, 2.87, 3.02, 2.97, 2.68, 2.75, 2.57, 2.61,
  2.9, 2.71, 2.74, 2.78, 2.82, 3.06, 3.52, 3.75, 4.52, 4.96,
  5.59, 5.74, 5.42, 5.31, 6.18, 5.38, 5.02, 4.89, 5.2, 5.03,
  6.0, 6.69, 6.5, 6.66, 7.07, 8.46, 9.74, 11.25, 11.61, 14.39,
  12.58, 12.34, 13.58, 13.44, 12.58, 16.29, 16.35, 16.38, 13.67, 13.78,
  12.47, 14.39, 15.93, 13.86, 14.36, 12.69, 13.17, 15.0, 12.65, 15.24,
  15.85, 19.14, 21.08, 25.06, 21.34, 21.15, 21.77, 19.24, 21.18, 24.56,
  29.27, 26.46, 27.58, 27.32, 29.32, 35.25, 40.27, 47.02, 45.19, 44.44,
  47.65, 47.59, 53.4, 29.64, 32.09, 27.15, 21.45, 26.4, 20.39, 18.78,
  22.06, 19.29, 20.89, 21.29, 19.98, 14.61, 17.45, 23.36, 22.5, 25.06,
  20.43, 21.76, 20.48, 19.78, 13.08, 13.46, 11.95, 9.96, 12.41, 14.99,
  11.18, 11.24, 12.4, 11.7, 13.22, 14.98, 14.97, 17.91, 20.59, 19.82,
  23.73, 24.17, 23.09, 21.99, 21.07, 19.62, 18.56, 20.63, 19.94, 17.61,
  15.41, 14.52, 16.11, 16.23, 16.96, 16.28, 17.45, 16.9, 17.11, 19.68,
  19.0, 19.81, 18.83, 18.05, 17.21, 19.6, 18.34, 15.62, 15.21, 14.36,
  14.75, 13.37, 14.1, 13.35, 14.6, 15.35, 15.92, 16.05, 15.18, 15.72,
  14.97, 14.42, 16.21, 16.81, 17.99, 17.9, 19.6, 19.69, 20.48, 19.94,
  20.38, 16.13, 15.36, 16.29, 17.12, 17.93, 16.62, 17.17, 17.81, 14.58,
  12.48, 10.84, 11.51, 10.13, 10.11, 11.93, 12.52, 12.58, 13.24, 15.4,
  16.38, 15.77, 15.4, 15.59, 16.57, 15.76, 16.81, 18.25, 18.7, 17.66,
  16.03, 16.98, 14.68, 15.95, 16.66, 17.71, 17.61, 17.97, 18.13, 17.04,
  19.54, 19.15, 18.86, 19.0, 17.3, 18.34, 21.09, 21.59, 21.02, 22.9,
  23.49, 24.57, 24.82, 22.74, 23.45, 22.62, 22.04, 20.12, 19.2, 17.55,
  18.5, 18.87, 18.93, 19.8, 21.71, 22.22, 22.17, 21.36, 20.31, 21.18,
  22.61, 22.24, 24.21, 22.89, 23.32, 24.3, 25.13, 25.95, 29.35, 32.19,
  33.39, 33.29, 32.52, 35.85, 34.92, 31.8, 32.23, 30.31, 31.55, 33.64,
  29.7, 28.26, 28.09, 29.67, 33.33, 34.47, 34.15, 30.75, 29.59, 31.76];

var sp500_stock = [68.84, 69.96, 69.55, 65.24, 59.63, 54.75, 58.23, 59.12, 56.27, 56.52, 62.26,
  63.1, 66.2, 64.29, 66.57, 69.8, 70.8, 69.37, 69.13, 72.5, 71.7,
  74.01, 73.23, 75.02, 77.04, 77.8, 78.98, 79.46, 80.37, 81.69, 83.18,
  81.83, 84.18, 84.86, 84.42, 84.75, 87.56, 87.43, 86.16, 89.11, 88.42,
  84.12, 85.25, 87.17, 89.96, 92.42, 91.61, 92.43, 92.88, 91.22, 89.23,
  91.06, 86.13, 84.74, 83.6, 77.1, 76.56, 80.2, 80.45, 80.33, 86.61,
  86.78, 90.2, 94.01, 89.08, 90.64, 94.75, 93.64, 96.71, 93.3, 94.0,
  96.47, 92.24, 89.36, 90.2, 97.46, 98.68, 99.58, 97.74, 98.86, 102.67,
  103.41, 108.37, 103.86, 103.01, 98.13, 101.51, 103.69, 103.46, 97.71, 91.83,
  95.51, 93.12, 97.12, 93.81, 92.06, 85.02, 89.5, 89.63, 81.52, 76.55,
  72.72, 78.05, 81.52, 84.3, 83.25, 87.2, 92.15, 95.88, 96.75, 100.31,
  103.95, 99.63, 98.7, 95.58, 99.03, 98.34, 94.23, 93.99, 102.09, 103.94,
  106.57, 107.2, 107.67, 109.53, 107.14, 107.39, 111.09, 110.55, 111.58, 116.67,
  118.05, 116.03, 111.68, 111.52, 106.97, 104.95, 104.26, 108.22, 104.25, 108.43,
  108.29, 95.96, 97.55, 96.57, 96.22, 93.98, 90.31, 87.28, 86.0, 79.31,
  72.15, 63.54, 73.9, 69.97, 68.56, 76.98, 81.59, 83.36, 87.3, 91.15,
  95.19, 88.75, 86.88, 83.87, 89.04, 91.24, 90.19, 100.86, 99.71, 102.77,
  101.64, 100.18, 104.28, 103.44, 102.91, 105.24, 102.9, 102.1, 107.46, 102.03,
  99.82, 98.42, 98.44, 96.12, 100.48, 98.85, 96.77, 96.53, 92.34, 94.83,
  95.1, 89.25, 87.04, 89.21, 96.83, 97.24, 95.53, 100.68, 103.29, 102.54,
  93.15, 94.7, 96.11, 99.93, 96.28, 101.59, 101.76, 99.08, 102.91, 103.81,
  109.32, 109.32, 101.82, 106.16, 107.94, 114.16, 113.66, 102.09, 106.29, 111.24,
  114.24, 121.67, 122.38, 125.46, 127.47, 140.52, 135.76, 129.55, 131.27, 136.0,
  132.81, 132.59, 131.21, 130.92, 122.79, 116.18, 121.89, 126.35, 122.55, 120.4,
  113.11, 111.96, 116.44, 111.88, 109.61, 107.09, 119.51, 120.42, 133.72, 138.53,
  140.64, 145.3, 148.06, 152.96, 164.43, 162.39, 167.64, 162.56, 164.4, 166.07,
  163.55, 166.4, 164.93, 163.41, 157.06, 159.18, 160.05, 150.55, 153.18, 150.66,
  166.68, 166.1, 166.09, 163.58, 167.24, 179.63, 181.18, 180.66, 179.83, 189.55,
  191.85, 190.92, 188.63, 182.08, 189.82, 202.17, 211.28, 211.78, 226.92, 238.9,
  235.52, 247.35, 250.84, 236.12, 252.93, 231.32, 243.98, 249.22, 242.17, 274.08,
  284.2, 291.7, 288.36, 290.1, 304.0, 318.66, 329.8, 321.83, 251.79, 230.3,
  247.08, 257.07, 267.82, 258.89, 261.33, 262.16, 273.5, 272.02, 261.52, 271.91,
  278.97, 273.7, 277.72, 297.47, 288.86, 294.87, 309.64, 320.52, 317.98, 346.08,
  351.45, 349.15, 340.36, 345.99, 353.4, 329.08, 331.89, 339.94, 330.8, 361.23,
  358.02, 356.15, 322.56, 306.05, 304.0, 322.22, 330.22, 343.93, 367.07, 375.22,
  375.34, 389.83, 371.16, 387.81, 395.43, 387.86, 392.45, 375.22, 417.09, 408.78,
  412.7, 403.69, 414.95, 415.35, 408.14, 424.21, 414.03, 417.8, 418.68, 431.35,
  435.71, 438.78, 443.38, 451.67, 440.19, 450.19, 450.53, 448.13, 463.56, 458.93,
  467.83, 461.79, 466.45, 481.61, 467.14, 445.77, 450.91, 456.5, 444.27, 458.26,
  475.49, 462.71, 472.35, 453.69, 459.27, 470.42, 487.39, 500.71, 514.71, 533.4,
  544.75, 562.06, 561.88, 584.41, 581.5, 605.37, 615.93, 636.02, 640.43, 645.5,
  654.17, 669.12, 670.63, 639.95, 651.99, 687.33, 705.27, 757.02, 740.74, 786.16,
  790.82, 757.12, 801.34, 848.28, 885.14, 954.31, 899.47, 947.28, 914.62, 955.4,
  970.43, 980.28, 1049.34, 1101.75, 1111.75, 1090.82, 1133.84, 1120.67, 957.28, 1017.01,
  1098.67, 1163.63, 1229.23, 1279.64, 1238.33, 1286.37, 1335.18, 1301.84, 1372.71, 1328.72,
  1320.41, 1282.71, 1362.93, 1388.91, 1469.25, 1394.46, 1366.42, 1498.58, 1452.43, 1420.6,
  1454.6, 1430.83, 1517.68, 1436.51, 1429.4, 1314.95, 1320.28, 1366.01, 1239.94, 1160.33,
  1249.46, 1255.82, 1224.38, 1211.23, 1133.58, 1040.94, 1059.78, 1139.45, 1148.08, 1130.2,
  1106.73, 1147.39, 1076.92, 1067.14, 989.82, 911.62, 916.07, 815.28, 885.76, 936.31,
  879.82, 855.7, 841.15, 848.18, 916.92, 963.59, 974.5, 990.31, 1008.01, 995.97,
  1050.71, 1058.2, 1111.92, 1131.13, 1144.94, 1126.21, 1107.3, 1120.68, 1140.84, 1101.72,
  1104.24, 1114.58, 1130.2, 1173.82, 1211.92, 1181.27, 1203.6, 1180.59, 1156.85, 1191.5,
  1191.33, 1234.18, 1220.33, 1228.81, 1207.01, 1249.48, 1248.29, 1280.08, 1280.66, 1294.87,
  1310.61, 1270.09, 1270.2, 1276.66, 1303.82, 1335.85, 1377.94, 1400.63, 1418.3, 1438.24,
  1406.82, 1420.86, 1482.37, 1530.62, 1503.35, 1455.27, 1473.99, 1526.75, 1549.38, 1481.14,
  1468.36, 1378.55, 1330.63, 1322.7, 1385.59, 1400.38, 1280.0, 1267.38, 1282.83, 1166.36,
  968.75, 896.24, 903.25, 825.88, 735.09, 797.87, 872.81, 919.14, 919.32, 987.48,
  1020.62, 1057.08, 1036.19, 1095.63, 1115.1, 1073.87, 1104.49, 1169.43, 1186.69, 1089.41,
  1030.71, 1101.6, 1049.33, 1141.2, 1183.26, 1180.55, 1257.64, 1286.12, 1327.22, 1325.83,
  1363.61, 1345.2, 1320.64, 1292.28, 1218.89, 1131.42, 1253.3, 1246.96, 1257.6, 1312.41,
  1365.68, 1408.47, 1397.91, 1310.33, 1362.16, 1379.32, 1406.58, 1440.67, 1412.16, 1416.18,
  1426.19, 1498.11, 1514.68, 1569.19, 1597.57, 1630.74, 1606.28, 1685.73, 1632.97, 1681.55,
  1756.54, 1805.81, 1848.36, 1782.59, 1859.45, 1872.34, 1883.95, 1923.57, 1960.23, 1930.67,
  2003.37, 1972.29, 2018.05, 2067.56, 2058.9, 1994.99, 2104.5, 2067.89, 2085.51, 2107.39,
  2063.11, 2103.84, 1972.18, 1920.03, 2079.36, 2080.41, 2043.94, 1940.24, 1932.23, 2022.19];



/** Generates an array of year values. */
function year_range(start_year, start_month, length) {
  var years = [],
      current_year = start_year,
      current_month = start_month;

  for (var i = 0; i < length; i++) {
    years.push(current_year + (current_month - 1) / 12.0);

    // prepare for the next loop pass
    current_month++;
    if (current_month >= 13) {
      current_month = 1;
      current_year++;
    }
  }

  // for testing - the final "years" value should be the same
  // console.log(years[years.length - 1]);

  return years;
}

aapl_stock.x = year_range(1980, 12, aapl_stock.y.length);
csco_stock.x = year_range(1990, 4, csco_stock.y.length);
goog_stock.x = year_range(2004, 9, goog_stock.y.length);
hpq_stock.x = year_range(1962, 1, hpq_stock.y.length);
intc_stock.x = year_range(1980, 4, intc_stock.y.length);


// ********* Stock Market Data ***********************************
// Here is the combined data structure.
var stock_data = {
  "AAPL": aapl_stock,
  "CSCO": csco_stock,
  "GOOG": goog_stock,
  "HPQ": hpq_stock,
  "INTC": intc_stock
};


/********** Event Information *********************************/

// untility function
var util = {};

/** Compute a floating point year value. */
util.compute_year = function (year, month, day) {
  return (year + (month - 1) / 12 + day / 372);
  // note it's divide by 373, not 365, not 366 
  // this is to make Dec 31 compute to slightly less than 1
}

/** Wraps the string str inside <strong> */
util.heading = function (str) {
  return "<p><strong>" + str + "</strong></p>";
};

/** Wraps an array of lines inside <p>. 
 * @param {Array.<string>} lines */
util.paragraph = function (lines) {
  var return_val = "<p>";

  for (var i = 0; i < lines.length; i++)
    return_val += lines[i] + "<br />";
  
  return_val += "</p>";
  return return_val;
};

/** Wraps a string inside <p> tag. Generate <br /> as needed.
  * @param {string} str */
util.paragraph2 = function (width, str) {
  var return_val = "<p>";

  var i = 0;
  while (i < str.length) {
    // each pass through this while loop adds a line to "return_val"
    // the current line starts on str[i], and ends on str[end_of_line]
    var end_of_line = i + width;

    if (str[end_of_line] != ' ') {
      // str[end_of_line] is not a space, so 
      // find the next best place to end the line

      // search to the left:
      var end_of_line1 = end_of_line - 1;
      while (str[end_of_line1] != ' ' && end_of_line1 > i)
        end_of_line1--;

      // search to the right:
      var end_of_line2 = end_of_line + 1;
      while (str[end_of_line2] != ' ' && end_of_line2 < str.length - 1)
        end_of_line2++;

      // make a choice for "end_of_line"
      if (end_of_line1 === i) end_of_line = end_of_line2;
      else if ((end_of_line - end_of_line1) < (end_of_line2 - end_of_line))
        end_of_line = end_of_line1;
      else 
        end_of_line = end_of_line2;
    }

    // at this point, it's determined that the current line will last from
    // str[i] to str[end_of_line]
    var line = str.substring(i, end_of_line + 1);
    return_val += line;

    if (end_of_line < str.length - 1)
      return_val += '<br />';

    i = end_of_line + 1;
  }  

  return_val += "</p>";
  return return_val;
};


var events = {};
events.santa_clara = [
  {
    "name": "Pacific Wars",
    "start": util.compute_year(1942, 12, 7),
    "end": util.compute_year(1975, 4, 30),
    "y": 175000,
    "dx": -150, "dy": -30,
    "html": "<p><strong>Dec 7, 1941 ~ Sep 2, 1945 - WW2 in the Pacific <br />"
          + "Mar 31, 1946 ~ May 1, 1950 - Chinese Civil War <br />"
          + "Jun 25, 1950 ~ Jul 27, 1953 - Korean War <br />"
          + "Nov 1, 1955 ~ Apr 30, 1975 - Vietnam War "
          + "</strong></p>"
          + "<p>Three decades of war across the Pacific <br /> "
          + "spurred economic development of the US West Coast, <br />"
          + "creating a population boom. </p>"
  },
  {
    "name": "The Rise of High Tech",
    "start": util.compute_year(1960, 9, 27),
    "end": util.compute_year(2020, 1, 0),
    "y": 642000,
    "dx": -200, "dy": -30,
    "html": "<p> Santa Clara county is a world leader in computer <br />"
          + "hardware and software. Some of the largest and most <br />"
          + "successful companies are founded in the county."
          + "<p><strong>Sep 27, 1960 - Fairchild created the first IC <br />"
          + "1971 - Intel created the first microprocessor, the 4004 <br />"
          + "1976 - Apple founded <br />"
          + "1983 - Altera founded <br />"
          + "1984 - Cisco, Xilinx founded <br />"
          + "1993 - NVidia founded <br />"
          + "1998 - Google founded <br />"
          + " </strong></p>"
  }
];

// slightly modified event for the Santa Clara population vs USA population graph
events.santa_clara_vs_usa = JSON.parse(JSON.stringify(events.santa_clara));
events.santa_clara_vs_usa[0].y = 1400;
events.santa_clara_vs_usa[1].y = 5300;

// Asian Immigration graph events
events.asian_immigration = [
  {
    "name": "Hart–Celler Act",
    "start": util.compute_year(1959, 1, 0),
    "end": util.compute_year(1971, 1, 0),
    "y": 0.025,
    "dx": -50, "dy": -30,
    "html": util.heading("1965")
          + util.paragraph([
            "The Hart–Celler Act fundamentally changes the ",
            "immigration quotas. The previous immigration ",
            "laws highly favor immigrants from Western and ",
            "Northern Europe, making it hard for Asians ",
            "to legally enter the country."])
  },
  {
    "name": "Normalization of Relationships with China",
    "start": util.compute_year(1972, 1, 0),
    "end": util.compute_year(2000, 1, 0),
    "y": 0.30,
    "dx": -50, "dy": -30,
    "html": util.heading("1972 ~ 1990s")
            + util.paragraph([
              "During the Chinese Civil War, US supported",
              "the ROC, which eventually lost. From 1949",
              "until 1972, US and China were enemies and", 
              "have no diplomatic relationship."])
            + util.heading("1972 - President Nixon visited China")
            + util.paragraph([
              "Nixon met Chairman Mao within hours of ",
              "landing in Beijing. Relations began to ",
              "normalize, improving slowly but steadily. ",
              "Immigration from mainland China only became ",
              "significant starting in the 90s. By 2014, ",
              "China is the third largest trading partner ",
              "of US - after EU and Canada."])
  },
  {
    "name": "Fall of Saigon",
    "start": util.compute_year(1970, 1, 0),
    "end": util.compute_year(1980, 1, 0),
    "y": 0.14,
    "dx": -50, "dy": -30,
    "html": util.heading("1975")
          + util.paragraph(["During the Vietnamese civil war, US ",
            "supported South Vietnam, which ultimately ",
            "lost. Saigon was the capital of South Vietnam, ",
            "the fall of which marked the end of the ",
            "Vietnam War. "])
          + util.paragraph(["130,000 Vietnamese closely associated with the ",
            "US or the South Vietnamese regime were evacuated. ",
            "The great majority of these refugees settled ",
            "in the US."])
  },
  {
    "name": "H-1B Visas",
    "start": util.compute_year(1990, 1, 0),
    "end": util.compute_year(2010, 1, 0),
    "y": 0.17,
    "dx": -50, "dy": -30,
    "html": util.heading("1990 First H-1B visas issued.")
          + util.paragraph(["By 1990, the computer revolution was in full ",
              "swing. The H-1B visas added yet another way ",
              "for Asians to enter the US. The visa holders ",
              "can eventually become permanent residents, ",
              "and then become US citizens."])
          + util.paragraph(["In the year 2014, 124,326 new visas were issued. ",
              "For many years the most common country of ",
              "origin has been India, followed by China."])
  }
];


// Stock graph events
events.stocks = {};
events.stocks["AAPL"] = [
  {
    "name": "Jobs #1",
    "start": util.compute_year(1981, 1, 0),
    "end": util.compute_year(1985, 9, 17),
    "y": 0.4,
    "dx": -50, "dy": -30,
    "html": util.heading("First tenure of Steve Jobs")
          + util.heading("Founding of Apple ~ 1985")
          + util.paragraph2(50, "1976 - Apple founded by Steve Jobs, Steve Wozniak and Ronald Wayne")
          + util.paragraph2(50, "1983 - Lisa computer released. It was the first computer sold to the general public that had a GUI, but it was a commercial failure.")
          + util.paragraph2(50, "1984 - Macintosh released. It was a limited commercial success due to its high price.")
          + util.paragraph2(50, "1985 - Steve Jobs lost a power struggle to John Sculley and resigned from Apple.")
  },
  {
    "name": "Second tenure of Steve Jobs",
    "start": util.compute_year(1997, 2, 0),
    "end": util.compute_year(2011, 10, 5),
    "y": 0.54,
    "dx": -50, "dy": -30,
    "html": util.heading("1997 ~ 2011")
          + util.paragraph2(50, "1997 - Apple purchased NeXT and brought back Steve Jobs as an advisor. Microsoft made a $150 million investment in non-voting Apple stock.")
          + util.paragraph2(50, "2011 Oct 5 - Steve Jobs passed away.")
  },
  {
    "name": "Leadership in Mobile",
    "start": util.compute_year(2001, 1, 0),
    "end": util.compute_year(2016, 1, 0),
    "y": 14,
    "dx": -100, "dy": -30,
    "html": util.heading("2001 ~ 2016")
          + util.paragraph2(70, "2001 - iPod released. Initially sales were slow because it was only compatible with the Mac. Later it became extremely popular. By April 2007, Apple had sold one hundred million iPods.")
          + util.paragraph2(70, "2003 - iTunes Store opened. It became the largest music vendor in the US in 2008, and the largest music vendor in the world in 2010.")
          + util.paragraph2(70, "2007 - iPhone released. It's the first wide spread smart phone.")
          + util.paragraph2(70, "2010 - iPad released. It's the first wide spread tablet.")
          + util.paragraph2(70, "2015 - Apple Watch released.")
  }
];

events.stocks["CSCO"] = [
  {
    "name": "Explosive Growth of the Web",
    "start": util.compute_year(1996, 1, 0),
    "end": util.compute_year(2016, 1, 0),
    "y": 6,
    "dx": -50, "dy": -30,
    "html": util.heading("1996 ~ 2016")
          + util.paragraph2(50, "1996 - HTTP v1.0 released. Explosive growth of the Internet begins.")
  },
  {
    "name": "Hardware Dominance",
    "start": util.compute_year(1993, 1, 0),
    "end": util.compute_year(2001, 1, 0),
    "y": 1.2,
    "dx": -50, "dy": -30,
    "html": util.heading("1993 ~ 2001")
          + util.paragraph2(60, "1993 - Cisco 2500 series routers released. It would stay in production until 2001, which is a ridiculously long time during the fast paced 90s.")
          + util.paragraph2(60, "1996 - Juniper founded. In the following year, Juniper was able to raise $40 million from the top telecom companies of that time: Siemens, Ericsson, Nortel, 3Com, Qwest, and AT&T.")
          + util.paragraph2(60, "1999 June - Juniper IPO. ")
          + util.paragraph2(60, "2001 - Juniper controlled 1/3 of the market share for high-end routers, mostly at the expense of Cisco.")
  }
];

events.stocks["GOOG"] = [
  {
    "name": "Search Dominance",
    "start": util.compute_year(2004, 9, 1),
    "end": util.compute_year(2008, 9, 1),
    "y": 70,
    "dx": -50, "dy": -30,
    "html": util.heading("Most Popular Search Engine")
          + util.paragraph2(50, "1996 - Google Search began as a research project.")
          + util.paragraph2(50, "2009 - Google Search reached 65% market share.")
  },
  {
    "name": "Android",
    "start": util.compute_year(2005, 6, 0),
    "end": util.compute_year(2009, 6, 0),
    "y": 200,
    "dx": -50, "dy": -30,
    "html": util.heading("Most Popular Smartphone OS")
          + util.paragraph2(50, "2005 - Google acquired Android Inc.")
          + util.paragraph2(50, "2008 September - T-Mobile G1, the first Android smartphone, was released.")
          + util.paragraph2(50, "2014 June - Android has reached one billion active users. Smartphone market share estimates range between 70 to 80 percent, far ahead of Apple iOS.")
  },
  {
    "name": "Gmail",
    "start": util.compute_year(2004, 9, 1),
    "end": util.compute_year(2008, 9, 1),
    "y": 130,
    "dx": -50, "dy": -30,
    "html": util.heading("Most Popular Email")
          + util.paragraph2(50, "2004 - Gmail beta release. The initial storage of 1GB per user is much more than other free webmail accounts at that time, which tend to range from 2 to 4 MB.")
          + util.paragraph2(50, "2012 June - Gmail is the most popular email with 425 million active users.")
          + util.paragraph2(50, "2014 May - Gmail became the first Android app to reach one billion installations.")
  },
  {
    "name": "Chrome Browser",
    "start": util.compute_year(2008, 9, 0),
    "end": util.compute_year(2012, 9, 0),
    "y": 250,
    "dx": -50, "dy": -30,
    "html": util.heading("Most Popular Browser")
          + util.paragraph2(50, "2008 September - The first beta version of Chrome browser was released.")
          + util.paragraph2(50, "2012 September - StatCounter report that Google Chrome became the world's most popular browser, with 34% market share.")
          + util.paragraph2(50, "2016 - Microsoft is keeping its latest Edge browser for Windows 10 only. FireFox has performance problems, especially when hosting Flash. The future for Chrome has never been so bright.")
  }
];

events.stocks["HPQ"] = [
  {
    "name": "Calculators",
    "start": util.compute_year(1970, 1, 0),
    "end": util.compute_year(1980, 1, 0),
    "y": 0.35,
    "dx": -50, "dy": -30,
    "html": util.heading("1972 ~ 1979")
          + util.paragraph2(50, "HP was a leading scientific instrument company.")
          + util.paragraph2(50, "1972 - HP-35, the world's first scientific calculator, released.")
          + util.paragraph2(50, "1974 - HP-65, the first programmable calculator.")
          + util.paragraph2(50, "1979 - HP-28C, the first symbolic and graphing calculator.")
  },
  {
    "name": "Laser Printers",
    "start": util.compute_year(1984, 1, 0),
    "end": util.compute_year(2000, 1, 0),
    "y": 2.4,
    "dx": -50, "dy": -30,
    "html": util.heading("1984 ~ 2000")
          + util.paragraph2(50, "1984 - The first LaserJet released. This is the first laser printer for IBM computers.")
          + util.paragraph2(50, "1991 - LaserJet IIISi released. This is the first Ethernet network printer.")
          + util.paragraph2(50, "2000 December - HP has shipped 50 million LaserJet printers.")
  },
  {
    "name": "Hurd",
    "start": util.compute_year(2005, 1, 0),
    "end": util.compute_year(2010, 8, 6),
    "y": 9.6,
    "dx": -50, "dy": 20,
    "html": util.heading("Mark Hurd, CEO 2005 ~ 2010")
          + util.paragraph2(50, "Mark Hurd was known for aggressive cost cutting, which in the short run boosted financial results and lifted the stock price. However, HP's R&D became weaker than ever before.")
  },
  {
    "name": "Empire",
    "start": util.compute_year(2007, 1, 0),
    "end": util.compute_year(2012, 1, 0),
    "y": 19,
    "dx": -50, "dy": 20,
    "html": util.heading("Empire Building 2008 ~ 2011")
          + util.paragraph2(50, "2008 - EDS (Electronic Data Systems) for $13.9 billion.")
          + util.paragraph2(50, "2009 - 3Com for $2.7 billion.")
          + util.paragraph2(50, "2010 - Palm Inc. for $1.2 billion, 3PAR for $2.35 billion.")
          + util.paragraph2(50, "2011 - Autonomy for $10.2 billion.")
          + util.paragraph2(50, "2012 - HP took a $8.8 billion \"accounting charge\" for the Autonomy purchase.")
  },
  {
    "name": "Layoffs",
    "start": util.compute_year(2011, 1, 0),
    "end": util.compute_year(2016, 1, 0),
    "y": 10,
    "dx": -50, "dy": 20,
    "html": util.heading("2012 ~ 2016")
          + util.paragraph2(50, "May 23, 2012 - HP announced restructuring plans to lay off 27,000 employees.")
          + util.paragraph2(50, "In the ensuing years, this number grew and grew.")
          + util.paragraph2(50, "2015 September - HP announced another round of layoffs, estimated at 25,000 to 30,000 jobs. Total number of layoffs is now 80,000 to 85,000.")
  }
];

events.stocks["INTC"] = [
    {
      "name": "80386",
      "start": util.compute_year(1985, 10, 0),
      "end": util.compute_year(1990, 10, 0),
      "y": 0.63,
      "dx": -50, "dy": -30,
      "html": util.heading("1985 October - Intel releases the first 386 CPUs.")
            + util.paragraph2(60, "The CPU prior to the 80386 was the 80286, and Intel allowed multiple manufacturers to make it. There were 80286 CPUs made by IBM, AMD, Harris (Intersil), Siemens AG, and Fujitsu.")
            + util.paragraph2(60, "The 80286 was a 16 bit CPU, and the 80386 was the first 32 bit CPU for IBM PCs. The performance gain is therefore quite large, but in practice is limited by DOS issues.")
            + util.paragraph2(60, "Unlike the 80286, Intel tried hard to single source this CPU. For almost five years, the only 80386 CPUs were from Intel. AMD eventually released the Am386 in March 1991 after overcoming legal obstacles. But by then Intel was already releasing the next generation processor, the 80486.")
    },
    {
      "name": "Intel Inside",
      "start": util.compute_year(1991, 1, 0),
      "end": util.compute_year(1998, 1, 0),
      "y": 1.37,
      "dx": -50, "dy": -30,
      "html": util.heading("1991 ~ Present")
            + util.paragraph2(60, "For 20 years AMD was Intel's main competitor. For the same performance, AMD often offers lower cost CPUs. In 1991 Intel began the \"Intel Inside\" campaign to build brand loyalty. If consumers think Intel is superior, they will pay more for Intel CPUs. ")
            + util.paragraph2(60, "The \"Intel Inside\" campaign worked really well, especially in the 90s when computers were expensive and the lack of Internet access meant lack of alternative opinions. Even knowledgeable consumers pay some extra money to have \"Intel Inside\", just to be on the safe side.")
    },
    {
      "name": "Netburst",
      "start": util.compute_year(2000, 11, 0),
      "end": util.compute_year(2007, 1, 0),
      "y": 46,
      "dx": -150, "dy": 30,
      "html": util.heading("Nov 2000 ~ Jan 2007")
            + util.paragraph2(60, "The Netburst was a major design mistake by Intel.")
            + util.paragraph2(60, "For many years, a new CPU means higher operating frequency. The 80286 ran on 6 MHz to 25 MHz, the 80386 ran on 12 MHz to 40 MHz, and so on, until the Pentium III ran on 450 MHz to 1.4 GHz. The Netburst is a CPU architecture that allow Intel to produce very high frequency CPUs.")
            + util.paragraph2(60, "In practice this worked out poorly because these high frequency CPUs generate a lot of heat. On personal computers people have problem cooling the CPU. It's much worse in the server world, where hundreds of computers are packed into a small room. Hot CPU means extra air conditioning to cool it.")
            + util.paragraph2(60, "AMD gained market share, and stock valuation, at the expense of Intel during those years.")
    },
    {
      "name": "14nm",
      "start": util.compute_year(2012, 1, 0),
      "end": util.compute_year(2016, 1, 0),
      "y": 25,
      "dx": -250, "dy": 30,
      "html": util.heading("Intel achieves dominance over AMD")
            + util.paragraph2(70, "Intel has always been slightly ahead of AMD in terms of device fabrication, but by 2015, this has turned into a gigantic lead.")
            + util.paragraph2(70, "2012 April - Intel released Ivy Bridge CPU based on 22 nm technology.")
            + util.paragraph2(70, "2014 September - Intel released Core M CPU based on 14 nm technology.")
            + util.paragraph2(70, "What is AMD using? The 2015 AMD CPU, the Excavator, is using 28 nm.")
    }
];

