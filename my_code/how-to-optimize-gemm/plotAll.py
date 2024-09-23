import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 15})

fig, ax = plt.subplots(figsize=(15, 10))
x = [16,32, 64, 128, 256, 512, 1024, 2048]
gflops_naive  = [8.192000e-01,8.192000e-01,8.243522e-01,7.800454e-01,6.779906e-01,6.829046e-01,6.521227e-01,4.765040e-01]
gflops_cblas  = [1.036962e-02, 7.281778e+00, 2.912711e+01,3.919910e+01,  5.242880e+01, 6.582527e+01,7.019526e+01,6.849181e+01]
gflops_pthread = [1.462857e-02, 4.283399e-01, 1.524093e+00, 2.430072e+00, 2.890132e+00, 33.321111e+00, 2.787409e+00,2.362033e+00]
gflops_openmp = [6.301538e-01, 8.511169e-01, 8.295696e-01, 8.208031e-01, 8.213255e-01, 8.449870e-01, 7.875989e-01,6.235626e-01]


plt.plot(x, gflops_naive, label='naive', color='blue', linestyle='-', linewidth=3.0)
plt.plot(x, gflops_cblas, label='cblas', color='green', linestyle='-.', linewidth=3.0)
plt.plot(x, gflops_pthread, label='pthread', color='red', linestyle='--', linewidth=3.0)
plt.plot(x, gflops_openmp, label='openmp', color='purple', linestyle=':', linewidth=3.0)

plt.title('dgemm')
plt.xlabel('matrix size')
plt.ylabel('gflops')
plt.legend()

plt.savefig('result.png')
plt.show()