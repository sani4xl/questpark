import React from 'react';
const axios = require('axios');

//import './index.css';

const REGISTER_URL = "https://hp.questpark.com.ua/api/index.php";

class Register extends React.Component {


    constructor(props) {
        super(props);

        this.state = {
            teamName: ''
        }
        
    }

  
    register() {

        if (!this.state.teamName) {
            return;
        }
        
    axios.post(REGISTER_URL, {
        action: 'register',
        teamName: this.state.teamName
    
  })
  .then(function (response) {
    localStorage.setItem('gameId', response.gameId);
    localStorage.setItem('gems', response.gems);
  })
  .catch(function (error) {
    console.log(error);
  })
  .then(function () {
    // always executed
  });  
    }

    updateTeamName(teamName) {
        this.setState({teamName});
    }
    
    render() {
    
        return (
            <div>
                <div>
                    Название команды:
                </div>
                <div>
                    <input type="text" value={this.state.teamName || ''} onChange={(event) => this.updateTeamName(event.target.value)} />
                </div>
                <div>
                <button className="check-zelie-button" onClick={() => this.register()  }>Отправить заявку в Хогвартс</button>
                </div>
          </div>
          );
        }
}

export default Register;