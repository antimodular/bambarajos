class BeatDetect
{
  private int algorithm;
  private boolean isOnset;
  private FFT spect;
  private int bins;
  private float sampleRate;
  private int currFrame;
  private int valCnt;
  private float[] valGraph;
  
  // vars for sEnergy
  private float[] eBuffer;
  private float[] dBuffer;
  
  // vars for fEnergy
  private float[][] feBuffer;
  private float[][] fdBuffer;
  private boolean[] fIsOnset;
  private float[] varGraph;
  private int varCnt;
  private int feNum;
  
  int snareVal;
  int  snareThresh;
  BeatDetect()
  {
    init(1024, 44100);
  }
  
  BeatDetect(int s, float r)
  {
    init(s, r);
  }
  
  private void init(int s, float r)
  {
    bins = s;
    sampleRate = r;
    algorithm = 1;
    isOnset = false;
    currFrame = frameCount;
    spect = new FFT(bins*2);
    spect.damp(1.0);
    valCnt = varCnt = 0;
    valGraph = new float[width];
    varGraph = new float[width];
    // sEnergy vars: energy buffer, difference buffer
    eBuffer = new float[int(r/s)];
    dBuffer = new float[15];
    // fEnergy vars: number of averages, energy buffers, difference buffers, onset array
    detectDetail(64);
  }
  
  void detect(AudioInput in)
  {
    switch ( algorithm )
    {
      case 1: sEnergy(in); break;
      case 2: fEnergy(in); break;
    }
  }
  
  boolean isOnset()
  {
    return isOnset || isRange(0, feNum-1, feNum/4);
  }
  
  boolean isOnset(int i)
  {
    return fIsOnset[i];
  }
  
  boolean isKick()
  {
    return fIsOnset[0];
  }
 
  boolean isSnare()
  {
    int low = 1;
    int hi = int(feNum/3);
     snareThresh = int ( (hi-low)/3 );
    //return isRange(low, hi, thresh);
 
    
    int num = 0;
    for (int i = low; i < hi+1; i++) if ( isOnset(i) ) num++;
    
    snareVal = num;
    return num >  snareThresh;
   
  }
  
  boolean isHat()
  {
    int low = int(feNum/2);
    int hi = feNum-1;
    int thresh = int( (hi-low)/3 );
    return isRange(low, hi, thresh);
  }
  
  boolean isRange(int low, int high, int threshold)
  {
    int num = 0;
    for (int i = low; i < high+1; i++) if ( isOnset(i) ) num++;
    return num > threshold;
  }
  
  void detectMode( String algo )
  {
    if ( algo == "SOUND_ENERGY" ) 
    {
      algorithm = 1;
      fIsOnset = new boolean[feNum];
    }
    else if ( algo == "FREQ_ENERGY" ) 
    {
      algorithm = 2;
      valCnt = varCnt = 0;
      valGraph = new float[width];
      varGraph = new float[width];
    }      
  }
  
  void detectDetail( int num )
  {
    feNum = num;
    spect.averages(num);
    feBuffer = new float[num][int(sampleRate/bins)];
    fdBuffer = new float[num][15];
    fIsOnset = new boolean[num];
  }
    
  void drawGraph()
  {
    // draw valGraph
    for (int i = 0; i < valCnt; i++)
    {
      stroke(255);
      line(i, (height/2) - valGraph[i], i, (height/2) + valGraph[i]);
    }
    // draw varGraph
    for (int i = 0; i < varCnt-1; i++)
    {
      stroke(255);
      line(i, height - varGraph[i], i+1, height - varGraph[i+1]);
    }
    strokeWeight(5);
    for (int i = 0; i < fIsOnset.length; i++)
    {
      int c = (i%8 == 0) ? color(255, 0, 0) : color(255);
      stroke(c);
      if ( fIsOnset[i] ) line((i*10), height - 100, (i*10), height);
    }
  } 
  
  private void sEnergy(AudioInput in)
  {
    float instant = spect.getLevel(in)*100;
    // compute the average local energy
    float E = average(eBuffer);
    // compute the variance of the energies in eBuffer
    float V = variance(eBuffer, E);
    // compute C using a linear digression of C with V
    float C = (-0.0025714*V) + 1.5142857;
    float diff = max(instant - C*E, 0);
    pushVal(diff);
    // find the average of only the positive values in dBuffer
    float dAvg = specAverage(dBuffer);
    // filter negative values
    float diff2 = max(diff - dAvg, 0);
    pushVar(diff2);
    if ( isOnset && currFrame == frameCount ) isOnset = true;
    else if ( currFrame < frameCount && isOnset ) isOnset = false;
    else isOnset = diff2 > 0 && instant > 2;
    shift(eBuffer, instant);
    shift(dBuffer, diff);
    currFrame = frameCount;
  }
  
  private void fEnergy(AudioInput in)
  {
    float instant, E, V, C, diff, dAvg, diff2;
    spect.getSpectrum(in);
    for (int i = 0; i < feNum; i++)
    {
      instant = (i+1)*spect.averages[i]*2000;
      E = average(feBuffer[i]);
      V = variance(feBuffer[i], E);
      C = (-0.0025714*V) + 1.5142857;
      diff = max(instant - C*E, 0);
      dAvg = specAverage(fdBuffer[i]);
      diff2 = max(diff - dAvg, 0);
      if ( fIsOnset[i] && currFrame == frameCount ) fIsOnset[i] = true;
      else if ( currFrame < frameCount && fIsOnset[i] ) fIsOnset[i] = false;
      else fIsOnset[i] = diff2 > 0 && instant > 2;
      shift(feBuffer[i], instant);
      shift(fdBuffer[i], diff);
    }
    currFrame = frameCount;    
  }
  
  private void pushVal(float v)
  {
    // println(valCnt);
    if ( valCnt == width)
    {
      valCnt = 0;
      valGraph = new float[width];
    }
    valGraph[valCnt] = v;
    valCnt++;
  }
  
  private void pushVar(float v)
  {
    // println(valCnt);
    if ( varCnt == width)
    {
      varCnt = 0;
      varGraph = new float[width];
    }
    varGraph[varCnt] = v;
    varCnt++;
  } 
  
  private void shift(float[] arr, float val)
  {
    arraycopy(arr, 0, arr, 1, arr.length-1);
    arr[0] = val;
  }
  
  private float average(float[] arr)
  {
    float avg = 0;
    for (int i = 0; i < arr.length; i++) avg += arr[i];
    avg /= arr.length;
    return avg;
  }
  
  private float specAverage(float[] arr)
  {
    float avg = 0;
    float num = 0;
    for (int i = 0; i < arr.length; i++) 
    {
      if ( arr[i] > 0 ) 
      {
        avg += arr[i];
        num++;
      }
    }
    if ( num > 0 ) avg /= num;
    return avg;
  }
  
  private float variance(float[] arr, float val)
  {
    float V = 0;
    for (int i = 0; i < arr.length; i++) V += pow(arr[i]-val, 2);
    V /= arr.length;
    return V;
  }   
}
    
  
