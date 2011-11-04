public int sumOfPrimes(int n) {
  BitSet bitSet = new BitSet(n + 1);
  bitSet.set(0, n + 1);
  for (int i = 2, last = n / 2; i <= last && i > 0; i = bitSet.nextSetBit(i + 1)) {
    for (int j = i + i; j <= bitSet.length(); j += i) {
      bitSet.clear(j);
    }
  }
  System.out.println(bitSet.toString()); // Print all primes from 1 up to n

  int sum = 0;
  int index = 0;
  while ((index = bitSet.nextSetBit(++index)) > 0) {
    sum += index;
  }
  System.out.println(sum);
}
