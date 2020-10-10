import React from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';


const axios = require('axios');
const REGISTER_URL = "https://hp.questpark.com.ua/api/index.php";

class RegisterQR extends React.Component {


    constructor(props) {
        super(props);

        this.state = {
            teamCode: null
        }
        
    }

    componentDidMount() {
        const code = this.props.match.params.code;
        if (!code) {
            window.location.href = `/`;
            return;
        }

       this.register(code);
    
    }
  
    register(code) {
       
        this.setState({isSending: 'Чаклуємо...'});
        
    axios.post(REGISTER_URL, {
        action: 'register',
        teamCode: code
    
  })
  .then( (response) => {

      if (response.data.gameId) {
        localStorage.setItem('gameId', response.data.gameId);
      }

      if (response.data.teamName) {
        localStorage.setItem('teamName', response.data.teamName);
      }

      if (response.data.info) {
        localStorage.setItem('gameInfo', JSON.stringify(response.data.info));
      }

      window.location.href = `/task`;
  })
  .catch((error) => {
    console.log(error);
    this.setState({isSending: 'Такого факультету не існує!'});
    window.location.href = `/`;
   
  });
}


    
    render() {
    
        return (
            <div id="register-screen" className="screen">
               <div className="control-group">Чаклуємо...</div>
          </div>
          );
        }
}


export default withRouter(RegisterQR);