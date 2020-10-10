import React from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';
import GameInfo from '../helper/GameInfo';

import Client from '../helper/Client';

const client = new Client();


class QR extends React.Component {


    constructor(props) {
        super(props);

        this.state = {
            
        }
        
    }

    componentDidMount() {
        const code = this.props.match.params.code;
        if (!code) {
            window.location.href = `/`;
            return;
        }

        client.checkCode(code).then((response) => {
            this.setState({awaitText: 'Вдалося!'});

            if (response.data.info) {
                localStorage.setItem('gameInfo', JSON.stringify(response.data.info));
                window.location.href = `/task`;
            } else {
                window.location.href = `/`;
            }
           
        })
        .catch((error) => {
            window.location.href = `/`;
        }); 
    }

  
    render() {
    
        return (
            <div id="task-screen" className="screen info-screen">
                <div className="control-group">
                    <NavLink className="close-btn" to={`/`}></NavLink>
                    Перевіряємо...
                </div>
               
          </div>
          );
        }
}

export default withRouter(QR);