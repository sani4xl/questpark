package com.questpaklab.soundscreen;

import android.media.MediaPlayer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.lang.reflect.Field;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    ListView mainList;
    ArrayList<Integer> musicIds;
    ArrayList<String> musicNames;
    MediaPlayer mpMusic;
    MediaPlayer mpVoice;


    ListView voiceList;
    ArrayList<Integer> voiceIds;
    ArrayList<String> voiceNames;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainList = (ListView) findViewById(R.id.list_view);
        voiceList = (ListView) findViewById(R.id.voicesList);

        /*

        */
        System.out.print("hello");
        Field[] fields=R.raw.class.getFields();

        R.raw r = new R.raw() ;

        musicIds = new ArrayList<Integer>();
        musicNames = new ArrayList<String>();

        voiceIds = new ArrayList<Integer>();
        voiceNames = new ArrayList<String>();

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
            /*
            if( field.getName().matches("music__") ){
                musicIds.add( field.getInt( r )  );
            }*/


        }



        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, musicNames);
        mainList.setAdapter(adapter);
        mainList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override

            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                if(mpMusic != null) {
                    mpMusic.reset();// stops any current playing song
                }

                mpMusic = MediaPlayer.create(getApplicationContext(), musicIds.get(i) );

                mpMusic.start(); // starting mediaplayer

            }

        });


        ArrayAdapter<String> voiceAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, voiceNames);
        voiceList.setAdapter(voiceAdapter);
        voiceList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override

            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                if(mpVoice != null) {
                    mpVoice.reset();// stops any current playing song
                }

                mpVoice = MediaPlayer.create(getApplicationContext(), voiceIds.get(i) );

                mpVoice.start(); // starting mediaplayer

            }

        });

    }
}
