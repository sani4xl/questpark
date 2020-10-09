import React from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';
import GameInfo from '../helper/GameInfo';
import Gems from '../Gems/index';
import './index.css';

const gameInfo = new GameInfo();



const axios = require('axios');
const API_URL = "https://hp.questpark.com.ua/api/index.php";

class Game extends React.Component {


    constructor(props) {
        super(props);

        this.state = {
            teamName: localStorage.getItem('teamName') || "Маглы"
        }
        
    }

    componentDidMount() {
        
    }

    hasGems() {
        if (typeof this.state.gems == undefined) {
            return false;
        }
        return this.state.gems && this.state.gems.length > 0;
    }

   

    task() {

    }

   

    renderControls() {
        if (this.state.isSending) {
            return <div id="zakl-form" className="control-group">
                {this.state.awaitText}
            </div>
        }

        return  <div id="zakl-form" className="control-group">
            <div className="control-row">
                 Введите заклинание:
            </div>
            <div className="control-row">
                <input type="text" className="text" value={this.state.zakl || ''} onChange={(event) => this.updateZakl(event.target.value)} />
            </div>
            <div>
                {this.renderButton()}
            </div>
        </div>;
    }

    check() {

        if (!this.state.zakl) {
            return;
        }

        this.setState({isSending: true, awaitText: 'Проверяем...'});
        
        axios.post(API_URL, {
            action: 'validate',
            code: this.state.zakl,
            taskIndex: gameInfo.getTaskIndex(),
            gameId: gameInfo.getGameId(),
        })
        .then((response) => {
            this.setState({awaitText: 'Поздравляем!'});

            if (response.data.info) {
                localStorage.setItem('gameInfo', JSON.stringify(response.data.info));
            }

            if (response.data.lastGemName) {
                window.location.href = `/gem/${response.data.lastGemName}/new/true`;
            } else {
                alert("Ошибка! Обратитесь к ведущему!");
            }
           
        })
        .catch((error) => {
            this.setState({awaitText: 'Не верно! Попробуйте еще раз...'});
        })
        .then(() => {
            setTimeout(() => {
                this.setState({isSending: false, zakl: ''});
            }, 2000);
        });  
    }

    updateZakl(zakl) {
        this.setState({zakl});
    }

    renderButton() {
        if (!this.state.zakl) {
            return;
        }

        return <button className="btn" onClick={() => this.check()  }>Проверить заклинание</button>;
    }

    renderHeader() {
        
        return <div className="control-group">
                <div className="control-row">Команда "{this.state.teamName}"</div>
                <div>
                    <NavLink className="btn" to={`/task`}>Посмотреть задание</NavLink>
                </div>
            </div>
    }
   
    render() {
    
        return (
            <div id="gems-screen">
                {this.renderHeader()}
                <Gems />
                {this.renderControls()}


          </div>
          );
    }
}

export default Game;