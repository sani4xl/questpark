<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
        header('Access-Control-Allow-Origin: *');
        header('Access-Control-Allow-Methods: POST, GET, DELETE, PUT, PATCH, OPTIONS');
        header('Access-Control-Allow-Headers: token, Content-Type');
        header('Access-Control-Max-Age: 1728000');
        header('Content-Length: 0');
        header('Content-Type: text/plain');
        die();
}

require_once($_SERVER['DOCUMENT_ROOT'] . '/wp-blog-header.php');
header('Access-Control-Allow-Origin: *');
header('Content-Type: application/json');


global $GAME_TABLE;
$GAME_TABLE = 'wpq_game_lubava_team';

$json = file_get_contents('php://input');
$input = json_decode($json);
global $wpdb;


function findGameTeamByName($teamName = null) {
  global $wpdb, $GAME_TABLE;
  return $wpdb->get_row($wpdb->prepare("SELECT * FROM $GAME_TABLE WHERE team = %s", $teamName));
}

function findGameTeam($teamId = null) {
  global $wpdb, $GAME_TABLE;
  $query = $wpdb->prepare("SELECT * FROM $GAME_TABLE WHERE id = %d", $teamId);
  return $wpdb->get_row($query);
}

function updateTeamGameInfo($teamId, $gameInfo) {
  global $wpdb, $GAME_TABLE;
  $wpdb->update( 
	 $GAME_TABLE, 
	 array( 
		'game_info' => json_encode($gameInfo),
		'updated_at' => date("Y-m-d H:i:s") 
	 ), 
	 array( 'id' => $teamId ), 
	 array( 
		'%s',	
		'%s'	
	 ), 
	 array( '%d' ) 
  );
}

function updateTeamFinish($teamId) {
  global $wpdb, $GAME_TABLE;
  $wpdb->update( 
	 $GAME_TABLE, 
	 array( 
		'finish_at' => date("Y-m-d H:i:s") 
	 ), 
	 array( 'id' => $teamId ), 
	 array( 
		'%s'	
	 ), 
	 array( '%d' ) 
  );
}

function getGemByCode($code) {
  $codes = [
    '1' => 'sole',
    '2' => 'mind',
    '3' => 'power',
    '4' => 'time',
    '5' => 'space',
    '6' => 'reality'
  ];
  
  return $codes[(string)$code];
}

if ($input->action == 'check') {
  $result = ['teamName' => $input->teamName, 'teamId' => $input->teamId];
  $team = findGameTeam($input->teamId);
  if (!$team) {
    exit;
  }
  $gameInfo = json_decode($team->game_info);
  $result['gems'] = $gameInfo->gems;
} 
elseif ($input->action == 'validate') {
  $gem = getGemByCode($input->code);
  if (!$gem) {
    header("HTTP/1.0 404 Not Found");
    exit;
  }
  $result = ['teamName' => $input->teamName, 'teamId' => $input->teamId];
  $team = findGameTeam($input->teamId);
  if (!$team) {
    exit;
  }
  
  
  if($gem) {
    $gameInfo = json_decode($team->game_info, true);
    if (!$gameInfo) {
      $gameInfo = [];
    }
    if (!$gameInfo['gems']) {
      $gameInfo['gems'] = [];
    }
    
    $gameInfo['gems'][$gem] = true;
    
    updateTeamGameInfo($input->teamId, $gameInfo);
    
    $result['found_gem'] = $gem;
  }
  
}
elseif ($input->action == 'snap') {
  $result = ['teamName' => $input->teamName, 'teamId' => $input->teamId];
  $team = findGameTeam($input->teamId);
  if (!$team) {
    exit;
  }
  
  
  if($team) {
    $gameInfo = json_decode($team->game_info, true);
    if ($gameInfo && $gameInfo['gems']) {
      
      if (count($gameInfo['gems']) >= 6) {
        updateTeamFinish($input->teamId);
      }
      $result['snaped'] = true;
    }
  }
  
  
}
elseif ($input->action == 'register') {
  $teamName = esc_sql($input->teamName);
  $team = findGameTeamByName($teamName);
  if (!$team) {
    $wpdb->insert( $GAME_TABLE, array('team'=> $teamName, 'created_at' => date("Y-m-d H:i:s")), array('%s', '%s') );
    $teamId = $wpdb->insert_id;
    $result = [
      'teamName' => $teamName,
      'teamId' => $teamId,
      'gems' => []
    ];
    
  }
}

header("HTTP/1.1 200 OK");
echo json_encode($result);


