import * as fs from 'fs';


class MatrixMultiplier {

  static OnMult(m_ar: number, m_br: number, benchmark: boolean): void {
    let pha: number[][] = Array.from({ length: m_ar }, () => Array(m_ar).fill(1.0));
    let phb: number[][] = Array.from({ length: m_br }, (_, i) => Array(m_br).fill(i + 1.0));
    let phc: number[][] = Array.from({ length: m_ar }, () => Array(m_br).fill(0.0));

    const start_time = Date.now();

    for (let i = 0; i < m_ar; i++) {
      for (let j = 0; j < m_br; j++) {
        let temp = 0;
        for (let k = 0; k < m_ar; k++) {
          temp += pha[i][k] * phb[k][j];
        }
        phc[i][j] = temp;
      }
    }

    const end_time = Date.now();

    if (benchmark) {
      console.log((end_time - start_time) / 1000 + ", " + m_ar);
    } else {
      console.log("Time: " + (end_time - start_time) / 1000 + " seconds");

      console.log("Result matrix:");
      for (let i = 0; i < Math.min(1, m_ar); i++) {
        for (let j = 0; j < Math.min(10, m_br); j++) {
          process.stdout.write(phc[i][j] + " ");
        }
        console.log();
      }
    }
  }

  static OnMultLine(m_ar: number, m_br: number, benchmark: boolean): void {
    let pha: number[][] = Array.from({ length: m_ar }, () => Array(m_ar).fill(1.0));
    let phb: number[][] = Array.from({ length: m_br }, (_, i) => Array(m_br).fill(i + 1.0));
    let phc: number[][] = Array.from({ length: m_ar }, () => Array(m_br).fill(0.0));

    const start_time = Date.now();

    for (let i = 0; i < m_ar; i++) {
      for (let k = 0; k < m_br; k++) {
        for (let j = 0; j < m_br; j++) {
          phc[i][j] += pha[i][k] * phb[k][j];
        }
      }
    }

    const end_time = Date.now();

    if (benchmark) {
      console.log((end_time - start_time) / 1000 + ", " + m_ar);
    } else {
      console.log("Time: " + (end_time - start_time) / 1000 + " seconds");

      console.log("Result matrix:");
      for (let i = 0; i < Math.min(1, m_ar); i++) {
        for (let j = 0; j < Math.min(10, m_br); j++) {
          process.stdout.write(phc[i][j] + " ");
        }
        console.log();
      }
    }
  }

  static run_benchmarks(n_tries: number): void {
    let size = 600;
    // while (size <= 3000) {
    //   for (let i = 0; i < n_tries; i++) {
    //     MatrixMultiplier.OnMult(size, size, true);
    //   }
    //   size += 400;
    // }

    size = 600;
    while (size <= 3000) {
      for (let i = 0; i < n_tries; i++) {
        MatrixMultiplier.OnMultLine(size, size, true);
      }
      size += 400;
    }
  }

  static main(): void {
    const readline = require('readline').createInterface({
      input: process.stdin,
      output: process.stdout
    });

    readline.question("\n1. Multiplication\n2. Line Multiplication\n3. Run Benchmarks and Output to File\nSelection?: ", (op: string) => {
      const operation = parseInt(op);

      if (operation === 0) {
        readline.close();
        return;
      }

      readline.question("Dimensions: lins=cols ? (ignore if benchmarking) ", (dimensions: string) => {
        const lin = parseInt(dimensions);
        const col = lin;

        if (operation === 1) {
          MatrixMultiplier.OnMult(lin, col, false);
        } else if (operation === 2) {
          MatrixMultiplier.OnMultLine(lin, col, false);
        } else if (operation === 3) {
          MatrixMultiplier.run_benchmarks(5);
        }

        readline.close();
      });
    });
  }
}

// MatrixMultiplier.main();
// PARA FAZER OS BENCHMARKS DESCOMENTA-SE PARTE DA FUNÇÃO e mete-se > docs/nome_do_ficheiro.csv
MatrixMultiplier.run_benchmarks(5);

