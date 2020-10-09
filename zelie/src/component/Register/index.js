import React from 'react';
import './index.css';

const axios = require('axios');
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

        const props = this.props;

        this.setState({isSending: true});
        
    axios.post(REGISTER_URL, {
        action: 'register',
        teamName: this.state.teamName
    
  })
  .then(function (response) {
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

    renderButton() {
        if (!this.state.teamName) {
            return;
        }

        return <button className="check-zelie-button btn" onClick={() => this.register()  }>Відправити звернення</button>;
    }

    renderControls() {
        if (this.state.isSending) {
            return <div className="control-group">Чаклуємо...</div>
        }

        return  <div className="control-group">
        <div className="control-row">
            Код або назва факультету Хогвартс:
        </div>
        <div className="control-row">
            <input type="text" className="text" value={this.state.teamName || ''} onChange={(event) => this.updateTeamName(event.target.value)} />
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