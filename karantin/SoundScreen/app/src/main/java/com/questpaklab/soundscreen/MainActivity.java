package com.questpaklab.soundscreen;

import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Environment;
import android.os.storage.StorageVolume;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.io.File;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    ListView mainList;
    ArrayList<String> musicPathList;
    ArrayList<String> musicNameList;
    MediaPlayer mpMusic;
    MediaPlayer mpVoice;


    ListView voiceList;
    ArrayList<String> voicePathList;
    ArrayList<String> voiceNameList;

    Integer currentMusicTrack = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainList = (ListView) findViewById(R.id.list_view);
        voiceList = (ListView) findViewById(R.id.voicesList);

       // File sdCardPath = Environment.getExternalStorageDirectory();
        //String secStore = System.getenv("SECONDARY_STORAGE");
        //Log.i("suka", Environment.getExternalStorageDirectory().getAbsolutePath() );

        // getting list of sd cards
        /*
        List<StorageUtils.StorageInfo> storageInfoList = StorageUtils.getStorageList();

        for (StorageUtils.StorageInfo temp : storageInfoList) {
            Log.i("suka" , temp.path );
        }
        */

        voicePathList = new ArrayList<String>();
        voiceNameList = new ArrayList<String>();

        musicPathList = new ArrayList<String>();
        musicNameList = new ArrayList<String>();

        String sdCardPath = "/mnt/external_sd";


        String voicesPath = Environment.getExternalStorageDirectory().getPath();
        File voicesDir = new File(sdCardPath + "/sounds/voices");
        final File[] files = voicesDir.listFiles();
        if ( files != null ) {
            for (File file : files) {
                if (file != null  && !file.isDirectory() ) {
                    Log.i("sd_card_path", file.getPath());
                    voicePathList.add(file.getPath() );
                    voiceNameList.add(file.getName().replaceAll("\\.mp3$",""));
                }
            }
        }


        String musicPath = Environment.getExternalStorageDirectory().getPath();
        File musicDir = new File(sdCardPath + "/sounds/music");
        final File[] musicFiles = musicDir.listFiles();
        if ( musicFiles != null ) {
            for (File file : musicFiles) {
                if (file != null  && !file.isDirectory() ) {
                    Log.i("sd_card_path", file.getPath());
                    musicPathList.add(file.getPath() );
                    musicNameList.add(file.getName().replaceAll("\\.mp3$","") );
                }
            }
        }

        //*/



        //System.out.print("hello");
        //Field[] fields=R.raw.class.getFields();

        //R.raw r = new R.raw() ;

        /*
        musicIds = new ArrayList<Integer>();
        musicNames = new ArrayList<String>();



        for(int count=0; count < fields.length; count++){

            Field field = fields[count] ;
            //System.out.print(fields[count].getName());
            try {

                //Log.i("Raw Asset haha: ", field.getName() );
                if( field.getName().matches("music__.*") ) {
                    int resourceID = field.getInt(r);


                    musicIds.add( Integer.valueOf(resourceID) );
                    musicNames.add( field.getName() );
                }

                else if( field.getName().matches("voice__.*") ) {
                    int resourceID = field.getInt(r);


                    voiceIds.add( Integer.valueOf(resourceID) );
                    voiceNames.add( field.getName() );
                }
            }
            catch(Exception e){
                Log.i("exception", e.getMessage() );
            }


        }
        */



        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, musicNameList);
        mainList.setAdapter(adapter);
        mainList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override

            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                currentMusicTrack = i;
                playNextMusicTrack();



            }

        });


        ArrayAdapter<String> voiceAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, voiceNameList);
        voiceList.setAdapter(voiceAdapter);
        voiceList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override

            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                if(mpVoice != null) {
                    mpVoice.reset();// stops any current playing song
                }

                mpVoice = MediaPlayer.create(getApplicationContext(), Uri.parse( voicePathList.get(i) ) );

                mpVoice.start(); // starting mediaplayer

            }

        });

    }

    public void playNextMusicTrack(){

        if(mpMusic != null) {
            mpMusic.reset();// stops any current playing song
        }

        mpMusic = MediaPlayer.create(getApplicationContext(), Uri.parse( musicPathList.get( currentMusicTrack ) ) );
        mpMusic.setOnCompletionListener(


        new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                try {
                    currentMusicTrack++;
                    if(currentMusicTrack >= musicPathList.size() ){
                        currentMusicTrack = 0;
                    }
                    playNextMusicTrack();

                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        );

        mpMusic.start(); // starting mediaplayer


    }

    public void onClickStopButton(View v) {
        if(mpVoice != null) {
            mpVoice.reset();// stops any current playing song
        }

        if(mpMusic != null){
            mpMusic.reset();
        }
    }
}
