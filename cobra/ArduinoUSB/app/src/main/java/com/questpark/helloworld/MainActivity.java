package com.questpark.helloworld;

import android.os.CountDownTimer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.media.MediaPlayer;
import android.view.View;
//import android.system.
import com.felhr.usbserial.UsbSerialDevice;
import com.felhr.usbserial.UsbSerialInterface;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.widget.EditText;

import java.io.UnsupportedEncodingException;
import java.util.Random;


public class MainActivity extends AppCompatActivity {
    public final String ACTION_USB_PERMISSION = "com.hariharan.arduinousb.USB_PERMISSION";
    private MediaPlayer mediaPlayer;
    UsbDevice device;
    UsbSerialDevice serialPort;
    UsbManager usbManager;
    UsbDeviceConnection connection;
    EditText mytext;
    boolean truthIsPlaying;
    boolean repeatAfterTruth;
    //int [] tracks;
    ArrayList<Integer> tracks;
    int currentTrack = 0;
    CountDownTimer playTimer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initializeViews();
        truthIsPlaying = false;
        repeatAfterTruth = true;
        mytext = (EditText) findViewById(R.id.mytext);
        tracks = new ArrayList<Integer>();//new int[3];
        tracks.add(0, R.raw.anekdot);
        tracks.add(1, R.raw.bezp);
        tracks.add(2, R.raw.boytsa);
        tracks.add(3, R.raw.cobray);
        tracks.add(4, R.raw.dolari);
        tracks.add(5, R.raw.gorod);
        tracks.add(6, R.raw.holodno);
        tracks.add(7, R.raw.homachki);
        tracks.add(8, R.raw.kofe);
        tracks.add(9, R.raw.krishka);
        tracks.add(10, R.raw.million);
        tracks.add(11, R.raw.mir);
        tracks.add(12, R.raw.miru);
        tracks.add(13, R.raw.mishami);
        tracks.add(14, R.raw.muki);
        tracks.add(15, R.raw.nenadete);
        tracks.add(16, R.raw.nevidite);
        tracks.add(17, R.raw.pandora);
        tracks.add(18, R.raw.pitat);
        tracks.add(19, R.raw.pokazhu);
        tracks.add(20, R.raw.prodazh);
        tracks.add(21, R.raw.prokl);
        tracks.add(22, R.raw.qp);
        tracks.add(23, R.raw.rukazhop);
        tracks.add(24, R.raw.sbegal);
        tracks.add(25, R.raw.staratsa);
        tracks.add(25, R.raw.strashno);
        tracks.add(27, R.raw.sudbu);
        tracks.add(28, R.raw.trupi);
        tracks.add(29, R.raw.utug);
        tracks.add(30, R.raw.zhdet);

        Collections.shuffle(tracks);


        usbManager = (UsbManager) getSystemService(this.USB_SERVICE);
        IntentFilter filter = new IntentFilter();
        filter.addAction(ACTION_USB_PERMISSION);
        filter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
        registerReceiver(broadcastReceiver, filter);


    }

    public void initSerial(){
        HashMap<String, UsbDevice> usbDevices = usbManager.getDeviceList();
        if (!usbDevices.isEmpty()) {
            boolean keep = true;
            for (Map.Entry<String, UsbDevice> entry:usbDevices.entrySet()) {
                device = entry.getValue();
                int deviceVID = device.getVendorId();
                System.out.println(deviceVID);
                mytext.append("device detected");
                if (deviceVID == 0x2341 || deviceVID == 0x6790 || deviceVID == 0x1A86 )//Arduino Vendor ID
                {
                    PendingIntent pi = PendingIntent.getBroadcast(this, 0,
                            new Intent(ACTION_USB_PERMISSION), 0);
                    usbManager.requestPermission(device, pi);
                    keep = false;
                } else {
                    mytext.append("not arduino");
                    mytext.append( String.valueOf(deviceVID  ) );
                    connection = null;
                    device = null;
                }

                if (!keep)
                    break;
            }
        }
        else{
            System.out.println("usb devices empty");
            mytext.append("no usb device");
        }
    }

    private final BroadcastReceiver broadcastReceiver = new BroadcastReceiver() { //Broadcast Receiver to automatically start and stop the Serial connection.
        @Override
        public void onReceive(Context context, Intent intent) {
            System.out.println("on receiv\n");
            if (intent.getAction().equals(ACTION_USB_PERMISSION)) {
                boolean granted =
                        intent.getExtras().getBoolean(UsbManager.EXTRA_PERMISSION_GRANTED);
                if (granted) {
                    connection = usbManager.openDevice(device);
                    serialPort = UsbSerialDevice.createUsbSerialDevice(device, connection);
                    if (serialPort != null) {
                        if (serialPort.open()) { //Set Serial Connection Parameters.
                            //setUiEnabled(true); //Enable Buttons in UI
                            serialPort.setBaudRate(9600);
                            serialPort.setDataBits(UsbSerialInterface.DATA_BITS_8);
                            serialPort.setStopBits(UsbSerialInterface.STOP_BITS_1);
                            serialPort.setParity(UsbSerialInterface.PARITY_NONE);
                            serialPort.setFlowControl(UsbSerialInterface.FLOW_CONTROL_OFF);
                            serialPort.read(mCallback); //
                            mytext.append("Serial Connection Opened!\n");
                            //tvAppend(textView,"Serial Connection Opened!\n");

                        } else {
                            System.out.println("SERIAL PORT NOT OPEN");
                            mytext.append("port not open");
                        }
                    } else {
                        System.out.println("SERIAL PORT IS NULL");
                        mytext.append("port is null");
                    }
                } else {
                    System.out.println("SERIAL PERM NOT GRANTED");
                    mytext.append("port not grandted");
                }
            }
            else{

                mytext.append("no permission");
            }
            /*
            else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_ATTACHED)) {
                onClickStart(startButton);
            } else if (intent.getAction().equals(UsbManager.ACTION_USB_DEVICE_DETACHED)) {
                onClickStop(stopButton);
            }*/
        };
    };

    UsbSerialInterface.UsbReadCallback mCallback = new UsbSerialInterface.UsbReadCallback() {
        //Defining a Callback which triggers whenever data is read.
        @Override
        public void onReceivedData(byte[] arg0) {
            String data = null;
            try {
                data = new String(arg0, "UTF-8");
                //data.concat("/n");
                //System.out.println(data);
                //mytext.append(data);
                tvAppend(mytext, "new string:\n");
                tvAppend(mytext, data);
                //tvAppend(mytext, String.valueOf (data.toLowerCase().indexOf("parol")) );
                if(!truthIsPlaying && data.toLowerCase().indexOf("@") >= 0 ){
                    truthIsPlaying = true;
                    playMain();
                }
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
    };

    private void tvAppend(EditText tv, CharSequence text) {
        final EditText ftv = tv;
        final CharSequence ftext = text;

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ftv.append(ftext);
            }
        });
    }

    public void initializeViews() {


    }

    public void initUsb(View view) {

        System.out.println("init serial");
        mytext.append("init serial");
        initSerial();
    }

    public void play(View view) {
        playMusic();
    }



    public void playMusic(){

        stopMusic();
        mediaPlayer = MediaPlayer.create(MainActivity.this, tracks.get(currentTrack) );

        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                stopMusic();

                currentTrack++;
                if(currentTrack >= tracks.size() ){
                    currentTrack = 0;
                }
                //currentTrack = currentTrack + 1;
                //if(currentTrack)
                //new Random().nextInt(tracks.size());

                System.out.println(currentTrack);

                playTimer = new CountDownTimer(60000, 1000) {
                    public void onFinish() {
                        // When timer is finished
                        // Execute your code here
                        System.out.println("delay timer finished");
                        playMusic();
                    }

                    public void onTick(long millisUntilFinished) {
                        // millisUntilFinished    The amount of time until finished.
                    }
                }.start();
            }
        });

        mediaPlayer.start();

        System.out.println("play");
    }

    public void playPass(View view) {
      playMain();
    }

    public void playMain(){
        stopMusic();
        mediaPlayer = MediaPlayer.create(MainActivity.this, R.raw.parol);
        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                truthIsPlaying = false;
                stopMusic();

                if(repeatAfterTruth) {
                    playTimer = new CountDownTimer(60000, 1000) {
                        public void onFinish() {
                            // When timer is finished
                            // Execute your code here
                            System.out.println("delay timer finished");
                            playMusic();
                        }

                        public void onTick(long millisUntilFinished) {
                            // millisUntilFinished    The amount of time until finished.
                        }
                    }.start();
                }
            }
        });
        mediaPlayer.start();

    }

    public void stop(View view) {
        stopMusic();
    }

    public void switchRepeat(View view) {
        repeatAfterTruth = !repeatAfterTruth;
    }

    protected void stopMusic(){
        if( playTimer != null){
            playTimer.cancel();

        }

        if( mediaPlayer != null ) {
            if(mediaPlayer.isPlaying()){
                mediaPlayer.stop();
            }
            mediaPlayer.reset();
            mediaPlayer.release();
            mediaPlayer = null;
        }
    }

}
