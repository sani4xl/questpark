import React, { Component } from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';

import GameInfo from '../helper/GameInfo';
import './index.css';

const gameInfo = new GameInfo();


class Gems extends Component {
    constructor(props) {
        super(props);
        this.state = {
            
        };
    }

    componentDidMount() {
        console.log(gameInfo.getGems());
        this.setState({gems: gameInfo.getGems()});
    }

   
    items() {
        if (!this.state.gems) {
            return;
        }
        console.log(Object.values(this.state.gems));
        return Object.values(this.state.gems).map((gem, index) => {
            console.log(gem);
            return <div key={index} className="gem-wrap">
                <NavLink className="gem" to={`/gem/${gem.name}`}>
                    <img src={gem.icon} />
                </NavLink>

            </div>
        });
    }


    render() {
    
        if (!gameInfo.hasGems()) {
            return <div className="control-group" id="no-gems">У вас еще нет магических вещей</div>;
        }

        return <div className="gems">
            {this.items()}
        </div>;
    }
}

export default Gems;