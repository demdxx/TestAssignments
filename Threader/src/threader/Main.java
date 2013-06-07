package threader;

public class Main {
  
  private static void message(String msg) {
    System.out.print(msg+"\n");
  }
  
  private static class ProcessController implements Runnable {
    
    private static final int MAX_COUNTER = 100;
    private volatile static int counter = 0;
    private Thread tCounter = null;
    private Thread tProcess = null;

    private static class CounterLoop implements Runnable {
      @Override
      public void run() {
        try {
          while (counter < MAX_COUNTER) {
            message("Status: "+counter);
            // Pause for 0.1 second
            Thread.sleep(100);
          }
        } catch (InterruptedException e) {
          message("Something wrong in counter!");
        }
      }
    }

    private static class ProcessLoop implements Runnable {
      @Override
      public void run() {
        try {
          for (; counter < MAX_COUNTER; counter++) {
            // Pause for 0.1 second
            Thread.sleep(100);
          }
        } catch (InterruptedException e) {
          message("I wasn't done!");
        }
      }
    }

    @Override
    public void run() {
      counter = 0;
      tCounter = new Thread(new CounterLoop());
      tProcess = new Thread(new ProcessLoop());

      tCounter.start();
      tProcess.start();
      
      try {
        while (tCounter.isAlive()) {
          Thread.sleep(1000);
        }
      } catch (InterruptedException e) {
        message("Something wring!");
      }
      if (tCounter.isAlive())
        tCounter.destroy();
      if (tProcess.isAlive())
        tProcess.destroy();
      tCounter = null;
      tProcess = null;
    }
  }
  
  public static void main(String[] args) {
    message("Start threader");

    Thread t = new Thread(new ProcessController());
    t.start();
    Thread t2 = new Thread(new ProcessController());
    t2.start();
    
    try {
      while (t.isAlive() && t2.isAlive()) {
        Thread.sleep(1000);
      }
      message("Processing complete!");
    } catch (InterruptedException e) {
      message("Processing complete error!");
    }
  }
}
