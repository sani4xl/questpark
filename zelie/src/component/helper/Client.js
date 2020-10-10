import GameInfo from './GameInfo';

const gameInfo = new GameInfo();
const axios = require('axios');
const API_URL = "https://hp.questpark.com.ua/api/index.php";

class Client {

    checkCode(code) {
        return axios.post(API_URL, {
            action: 'validate',
            code: code,
            taskIndex: gameInfo.getTaskIndex(),
            gameId: gameInfo.getGameId(),
        });
    }
    
}

export default Client;