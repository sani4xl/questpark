import React, { Component } from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';

import GameInfo from '../helper/GameInfo';


const gameInfo = new GameInfo();


class Admin extends Component {
    constructor(props) {
        super(props);
        this.state = {
            
        };
    }

   

    renderCloseButton() {
        return <NavLink className="close-btn" to={`/`}></NavLink>;
    }

    resetGame() {
        if (window.confirm("Ви дійсно бажаєте зкинути гру на цьому девайсі?")) {
            localStorage.removeItem('gameId');
            localStorage.removeItem('teamName');
            localStorage.removeItem('gameInfo');
            window.location.href = `/`;
        }
    }


    render() {
    
        return (
            <div id="gem-screen" className="screen info-screen">
                <div className="control-group">
                    {this.renderCloseButton()}
                    <div>
                        <button className="btn" onClick={() => this.resetGame()}>Зкинути гру</button>
                        
                    </div>
                </div>
               
          </div>
          );
        }
}

export default withRouter(Admin);