import React, { Component } from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';

//import './index.css';
import Register from '../Register/index';
import Game from '../Game/index';


class Main extends Component {
    constructor() {
        super();
        this.state = {
            gameId: localStorage.getItem("gameId")
        };
    }

    registerGame(gameId) {
        this.setState({gameId});
    }

    renderGems() {
        if (this.state.gameId) {
            return <Game />;
        }
        return <Register onRegistration={(gameId) => this.registerGame(gameId)} />;
    }

    render() {
        return (
            <div>
                {this.renderGems()}
            </div>
        )
    }
}

export default withRouter(Main);