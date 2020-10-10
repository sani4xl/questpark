import React from 'react';
import './index.css';

const axios = require('axios');
const REGISTER_URL = "https://hp.questpark.com.ua/api/index.php";

class Register extends React.Component {


    constructor(props) {
        super(props);

        this.state = {
            teamCode: null
        }
        
    }

  
    register() {

        if (!this.state.teamCode) {
            return;
        }

        const props = this.props;

        this.setState({isSending: 'Чаклуємо...'});
        
    axios.post(REGISTER_URL, {
        action: 'register',
        teamCode: this.state.teamCode
    
  })
  .then( (response) => {
        //this.setState({isSending: null});

      if (response.data.gameId) {
        localStorage.setItem('gameId', response.data.gameId);
      }

      if (response.data.teamName) {
        localStorage.setItem('teamName', response.data.teamName);
      }

      if (response.data.info) {
        localStorage.setItem('gameInfo', JSON.stringify(response.data.info));
      }

      if (props.onRegistration) {
        props.onRegistration(response.data.gameId);
      }
  })
  .catch((error) => {
    console.log(error);
    this.setState({isSending: 'Такого факультету не існує!'});

   
  })
  .then( ()  => {
    setTimeout(() => {
        this.setState({isSending: null});
    }, 3000);
  });  
    }

    updateTeamCode(teamCode) {
        this.setState({teamCode});
    }

    renderButton() {
        if (!this.state.teamCode) {
            return;
        }

        return <button className="check-zelie-button btn" onClick={() => this.register()  }>Відправити звернення</button>;
    }

    renderControls() {
        if (this.state.isSending) {
            return <div className="control-group">{this.state.isSending}</div>
        }

        return  <div className="control-group">
        <div className="control-row">
            Введіть код факультету Хогвартс:
        </div>
        <div className="control-row">
            <input type="text" className="text" value={this.state.teamCode || ''} onChange={(event) => this.updateTeamCode(event.target.value)} />
        </div>
        <div>
            {this.renderButton()}
        </div>
        </div>;
    }
    
    render() {
    
        return (
            <div id="register-screen" className="screen">
               {this.renderControls()}
          </div>
          );
        }
}

export default Register;