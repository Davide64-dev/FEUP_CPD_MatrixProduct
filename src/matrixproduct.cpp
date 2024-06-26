#include <ctime>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <papi.h>

using namespace std;
using namespace std::chrono;

#define SYSTEMTIME clock_t

char *OnMult(int m_ar, int m_br, bool benchmark) {
  SYSTEMTIME Time1, Time2;
  char st[100];
  double temp;
  int i, j, k;

  double *pha, *phb, *phc;

  pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

  for(i=0; i<m_ar; i++) {
    for(j=0; j<m_ar; j++) {
      pha[i*m_ar + j] = (double)1.0;
    }
  }

  for(i=0; i<m_br; i++) {
    for(j=0; j<m_br; j++) {
      phb[i*m_br + j] = (double)(i+1);
    }
  }

  Time1 = clock();

  for(i=0; i<m_ar; i++) {
    for( j=0; j<m_br; j++) {
      temp = 0;
      for( k=0; k<m_ar; k++) {
        temp += pha[i*m_ar+k] * phb[k*m_br+j];
      }
      phc[i*m_ar+j]=temp;
    }
  }
  
  Time2 = clock();

  if (!benchmark) {
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;

    // display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; i++) {
      for (j = 0; j < min(10, m_br); j++) {
        cout << phc[j] << " ";
      }
    }
    cout << endl;
  } else {

    char *line = (char *)malloc(100);
    // format is: time, size, L1 misses, L2 misses (todo)
    sprintf(line, "%3.3f, %d", (double)(Time2 - Time1) / CLOCKS_PER_SEC, m_ar);
    return line;
  }

  free(pha);
  free(phb);
  free(phc);

  return nullptr;
}

// add code here for line x line matrix multiplication
char *OnMultLine(int m_ar, int m_br, bool benchmark) {
  SYSTEMTIME Time1, Time2;
  char st[100];
  int i, j, k;

  double *pha, *phb, *phc;

  pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

  for(i=0; i<m_ar; i++) {
    for(j=0; j<m_ar; j++) {
      pha[i*m_ar + j] = (double)1.0;
    }
  }

  for(i=0; i<m_br; i++) {
    for(j=0; j<m_br; j++) {
      phb[i*m_br + j] = (double)(i+1);
    }
  }

  for(i = 0; i < m_br; i++) {
    for(j = 0; j < m_br; j++) {
      phc[i * m_br + j] = (double)0.0;
    }
  }

  Time1 = clock();

  for (i = 0; i < m_ar; i++) {
    for (k = 0; k < m_br; k++) {
      for (j = 0; j < m_ar; j++) {
        phc[i * m_ar + j] += pha[i * m_ar + k] * phb[k * m_br + j];
      }
    }
  }

  Time2 = clock();
  sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
  cout << st;

  if (!benchmark) {
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;

    // display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; i++) {
      for (j = 0; j < min(10, m_br); j++) {
        cout << phc[j] << " ";
      }
    }
    cout << endl;
  } else {

    char *line = (char *)malloc(100);
    // format is: time, size, L1 misses, L2 misses (todo)
    sprintf(line, "%3.3f, %d", (double)(Time2 - Time1) / CLOCKS_PER_SEC, m_ar);
    return line;
  }

  free(pha);
  free(phb);
  free(phc);

  return nullptr;
}

// add code here for block x block matrix multiplication
char *OnMultBlock(int m_ar, int m_br, int bkSize, bool benchmark) {

  SYSTEMTIME Time1, Time2;
  char st[100];
  int i, j, k, l, m, n;

  double *pha, *phb, *phc;

  pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

  for(i=0; i<m_ar; i++) {
    for(j=0; j<m_ar; j++) {
      pha[i*m_ar + j] = (double)1.0;
    }
  }

  for(i=0; i<m_br; i++) {
    for(j=0; j<m_br; j++) {
      phb[i*m_br + j] = (double)(i+1);
    }
  }

  for(i = 0; i < m_br; i++) {
    for(j = 0; j < m_br; j++) {
      phc[i * m_br + j] = (double)0.0;
    }
  }

  Time1 = clock();

  // for loops to create the blox
  for (l = 0; l < m_ar; l += bkSize)
    for (m = 0; m < m_br; m += bkSize)
      for (n = 0; n < m_ar; n += bkSize)

        // matrix blocks multiplication
        for (i = l; i < min(l + bkSize, m_ar); i++)
          for (k = m; k < min(m + bkSize, m_br); k++)
            for (j = n; j < min(n + bkSize, m_ar); j++)
              phc[i * m_ar + j] += pha[i * m_ar + k] * phb[k * m_br + j];


  Time2 = clock();

  if (!benchmark) {
    sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
    cout << st;

    // display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for(i=0; i<1; i++) {
      for(j=0; j<min(10,m_br); j++) {
        cout << phc[j] << " ";
      }
    }
    cout << endl;
  } else {

    char *line = (char *)malloc(100);
    // format is: time, size, blk_size, L1 misses, L2 misses (todo)
    sprintf(line, "%3.3f, %d, %d", (double)(Time2 - Time1) / CLOCKS_PER_SEC, m_ar, bkSize);
    return line;
  }

  free(pha);
  free(phb);
  free(phc);

  return nullptr;
}

char *OnMultMultiOne(int m_ar, int m_br, bool benchmark) {
  high_resolution_clock::time_point time1, time2;
  char st[100];
  double temp;
  int i, j, k;

  double *pha, *phb, *phc;

  pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

  for(i=0; i<m_ar; i++) {
    for(j=0; j<m_ar; j++) {
      pha[i*m_ar + j] = (double)1.0;
    }
  }

  for(i = 0; i < m_br; i++) {
    for(j = 0; j < m_br; j++) {
      phb[i*m_br + j] = (double)(i+1);
    }
  }

  time1 = high_resolution_clock::now();

  #pragma omp parallel for
  for (i = 0; i < m_ar; i++) {
    for (k = 0; k < m_br; k++) {
      for (j = 0; j < m_ar; j++) {
        phc[i * m_ar + j] += pha[i * m_ar + k] * phb[k * m_br + j];
      }
    }
  }

  time2 = high_resolution_clock::now();

  if (!benchmark) {
    duration<double> elapsedTime = duration_cast<duration<double>>(time2 - time1);
    sprintf(st, "Time: %3.3f seconds\n", elapsedTime.count());
    cout << st;

    // Display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; i++) {
      for (j = 0; j < min(10, m_br); j++) {
        cout << phc[j] << " ";
      }
    }
    cout << endl;
  } else {
    char* line = (char*)malloc(100);
    // Format is: time, size, L1 misses, L2 misses (todo)
    duration<double> elapsedTime = duration_cast<duration<double>>(time2 - time1);
    sprintf(line, "%3.3f, %d", elapsedTime.count(), m_ar);
    return line;
  }

  free(pha);
  free(phb);
  free(phc);

  return nullptr;
}

// this is supposed to be slower
char *OnMultMultiTwo(int m_ar, int m_br, bool benchmark) {
  high_resolution_clock::time_point time1, time2;
  char st[100];
  double temp;
  int i, j, k;

  double *pha, *phb, *phc;

  pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
  phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

  for(i=0; i<m_ar; i++) {
    for(j=0; j<m_ar; j++) {
      pha[i*m_ar + j] = (double)1.0;
    }
  }

  for(i=0; i<m_br; i++) {
    for(j=0; j<m_br; j++) {
      phb[i*m_br + j] = (double)(i+1);
    }
  }

  time1 = high_resolution_clock::now();

  #pragma omp parallel private(i, k)
  for (i = 0; i < m_ar; i++) {
    for (k = 0; k < m_br; k++) {
      #pragma omp for
      for (j = 0; j < m_ar; j++) {
        phc[i * m_ar + j] += pha[i * m_ar + k] * phb[k * m_br + j];
      }
    }
  }
  
  time2 = high_resolution_clock::now();

  if (!benchmark) {
    duration<double> elapsedTime = duration_cast<duration<double>>(time2 - time1);
    sprintf(st, "Time: %3.3f seconds\n", elapsedTime.count());
    cout << st;

    // Display 10 elements of the result matrix to verify correctness
    cout << "Result matrix: " << endl;
    for (i = 0; i < 1; i++) {
      for (j = 0; j < min(10, m_br); j++) {
        cout << phc[j] << " ";
      }
    }
    cout << endl;
  } else {
    char* line = (char*)malloc(100);
    // Format is: time, size, L1 misses, L2 misses (todo)
    duration<double> elapsedTime = duration_cast<duration<double>>(time2 - time1);
    sprintf(line, "%3.3f, %d", elapsedTime.count(), m_ar);
    return line;
  }

  free(pha);
  free(phb);
  free(phc);

  return nullptr;
}

void handle_error (int retval) {
  printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
  exit(1);
}

void init_papi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT && retval < 0) {
    printf("PAPI library version mismatch!\n");
    exit(1);
  }

  if (retval < 0) handle_error(retval);

  std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
            << " MINOR: " << PAPI_VERSION_MINOR(retval)
            << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}

char *getPapiValues(int EventSet, long long values[]) {
  char *res = (char *)malloc(50);

  int ret = PAPI_stop(EventSet, values);
  if (ret != PAPI_OK) {
    cout << "ERROR: Stop PAPI" << endl;
    return nullptr;
  }

  sprintf(res, "%lld, %lld, %lld, %lld", values[0], values[1], values[2], values[3]);

  ret = PAPI_reset(EventSet);
  if (ret != PAPI_OK) {
    std::cout << "FAIL reset" << endl; 
    return nullptr;
  }

  // Start counting
  ret = PAPI_start(EventSet);
  if (ret != PAPI_OK) cout << "ERROR: Start PAPI" << endl;

  return res;
}

void write_to_file(string path, char *line) {
    ofstream out(path, ios::app);
    if (!out.is_open()) {
      cout << "Error opening file " << path << " for writing!" << endl;
      return;
    }   
    out << line << endl;
    out.close();
}

void run_benchmarks(int n_tries, int EventSet, long long values[]) {
  // run OnMult benchmarks
  int size = 600;
  while (size <= 3000) {
    for (int i = 0; i < n_tries; i++) {
      char *time_size = OnMult(size, size, true);
      char *papi_values = getPapiValues(EventSet, values);
      char line[100];
      sprintf(line, "%s, %s", time_size, papi_values);
      write_to_file("../docs/OnMult.csv", line);
    }
    size += 400;
  }

  // run OnMultLine benchmarks for sizes up to 3000 with 600 increments
  size = 600;
  while (size <= 3000) {
    for (int i = 0; i < n_tries; i++) {
      char *time_size = OnMultLine(size, size, true);
      char *papi_values = getPapiValues(EventSet, values);
      char line[100];
      sprintf(line, "%s, %s", time_size, papi_values);
      write_to_file("../docs/OnMultLine.csv", line);
    }
    size += 400;
  }

  // run OnMultLine benchmarks for sizes up to 10240 with 2048 increments starting at 4096
  size = 4096;
  while (size <= 10240) {
    for (int i = 0; i < n_tries; i++) {
      char *time_size = OnMultLine(size, size, true);
      char *papi_values = getPapiValues(EventSet, values);
      char line[100];
      sprintf(line, "%s, %s", time_size, papi_values);
      write_to_file("../docs/OnMultLine.csv", line);
    }
    size += 2048;
  }

  // run OnMultBlock benchmarks for sizes
  int blk_size_qnt = 3;
  const int init_blk_size = 128;
  int blk_size = 128;
  size = 100;
  while (size <= 10240) {
    blk_size = init_blk_size;
    while (blk_size <= blk_size_qnt * 2 * init_blk_size) {
      for (int i = 0; i < n_tries; i++) {
        char *time_size = OnMultBlock(size, size, blk_size, true);
        char *papi_values = getPapiValues(EventSet, values);
        char line[100];
        sprintf(line, "%s, %s", time_size, papi_values);
        write_to_file("../docs/OnMultBlock.csv", line);
      }
      blk_size *= 2;
    }
    size += 2048;
  }

  // run OnMultMultiOne benchmarks for sizes up to 10240
  size = 600;
  int increment = 400;
  while (size <= 10240) {
    for (int i = 0; i < n_tries; i++) {
      char *time_size = OnMultMultiOne(size, size, true);
      char *papi_values = getPapiValues(EventSet, values);
      char line[100];
      sprintf(line, "%s, %s", time_size, papi_values);
      write_to_file("../docs/OnMultMultiOne.csv", line);
    }

    if (size == 3000) {
      increment = 2048;
      size = 4096;
    }
    size += increment;
  }

  // run OnMultMultiTwo benchmarks for sizes up to 10240
  size = 600;
  increment = 2048;
  while (size <= 10240) {
    for (int i = 0; i < n_tries; i++) {
      char *time_size = OnMultMultiTwo(size, size, true);
      char *papi_values = getPapiValues(EventSet, values);
      char line[100];
      sprintf(line, "%s, %s", time_size, papi_values);
      write_to_file("../docs/OnMultMultiTwo.csv", line);
    }

    if (size == 3000) {
      increment = 2048;
      size = 4096;
    }
    size += increment;
  }
}

int main (int argc, char *argv[]) {
	char c;
	int lin, col, blockSize;
	int op;
	
	int EventSet = PAPI_NULL;
  long long values[2];
  int ret;

	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if (ret != PAPI_VER_CURRENT) {
		std::cout << "FAIL" << endl;
  }

	ret = PAPI_create_eventset(&EventSet);
  if (ret != PAPI_OK) cout << "ERROR: create eventset" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L1_DCM" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_DCM" << endl;
  
	ret = PAPI_add_event(EventSet,PAPI_L2_DCA);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_DCA" << endl;

	ret = PAPI_add_event(EventSet, PAPI_FP_OPS);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_FP_OPS" << endl;

	op = 1;
	do {
		cout << endl << "1. Multiplication" << endl;
		cout << "2. Line Multiplication" << endl;
		cout << "3. Block Multiplication" << endl;
		cout << "4. Multi Core Implementation 1" << endl;
    cout << "5. Multi Core Implementation 2" << endl;
    cout << "6. Run Benchmarks And Save Logs" << endl;
		cout << "Selection?: ";
		cin >>op;
		if (op == 0) break;
    
    if (op != 6) {
      printf("Dimensions: lins=cols ? ");
      cin >> lin;
      col = lin;
    }

		// Start counting
		ret = PAPI_start(EventSet);
		if (ret != PAPI_OK) cout << "ERROR: Start PAPI" << endl;

		switch (op) {
			case 1: 
				OnMult(lin, col, false);
				break;
			case 2:
				OnMultLine(lin, col, false);  
				break;
			case 3:
				cout << "Block Size? ";
				cin >> blockSize;
				OnMultBlock(lin, col, blockSize, false);  
				break;
      case 4:
        OnMultMultiOne(lin, col, false);
        break;
      case 5:
        OnMultMultiTwo(lin, col, false);
        break;
      case 6:
        cout << "Running benchmarks..." << endl;
        run_benchmarks(5, EventSet, values);
        break;
		}

    ret = PAPI_stop(EventSet, values);
    if (ret != PAPI_OK) cout << "ERROR: Stop PAPI" << endl;
    printf("L1 DCM: %lld \n",values[0]);
    printf("L2 DCM: %lld \n",values[1]);

		ret = PAPI_reset(EventSet);
		if (ret != PAPI_OK) {
			std::cout << "FAIL reset" << endl; 
    }
	} while (op != 0);

	ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
	if (ret != PAPI_OK) {
		std::cout << "FAIL remove event" << endl; 
  }

	ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
	if (ret != PAPI_OK) {
		std::cout << "FAIL remove event" << endl; 
  }

	ret = PAPI_remove_event( EventSet, PAPI_L2_DCA );
	if (ret != PAPI_OK) {
		std::cout << "FAIL remove event" << endl; 
  }

	ret = PAPI_remove_event( EventSet, PAPI_FP_OPS );
	if (ret != PAPI_OK) {
		std::cout << "FAIL remove event" << endl; 
  }

	ret = PAPI_destroy_eventset( &EventSet );
	if (ret != PAPI_OK) {
		std::cout << "FAIL destroy" << endl;
  }
}

