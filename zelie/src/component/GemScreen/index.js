import React, { Component } from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';

import GameInfo from '../helper/GameInfo';
import './index.css';

const gameInfo = new GameInfo();


class GemScreen extends Component {
    constructor(props) {
        super(props);
        this.state = {
            
        };
    }

    componentDidMount() {
        const gemName = this.props.match.params.gemName;
        if (gemName) {
            this.setState({gem: gameInfo.getGemByName(gemName)});
        }
    }

    rendeGem() {
        const gem = this.state.gem;
        if (!gem) {
            return;
        }


        return <div>
            <div class="center">
                <img className="gem-img" src={gem.icon} />
            </div>
            <h2 className="gem-title">{gem.title}</h2>
            <div className="gem-description">{gem.description}</div>
            {this.renderNextTask()}
        </div>

    }

    renderNextTask() {
        if (!this.props.match.params.new) {
            return;
        }
        return <NavLink className="btn" to={`/task`}>Наступне завдання</NavLink>;
    }

    renderCloseButton() {
        if (this.props.match.params.new) {
            return;
        }
        return <NavLink className="close-btn" to={`/`}></NavLink>;
    }

    getBgStyle() {
        
        const gem = this.state.gem;
        if (!gem || !gem.bg) {
            return;
        }

        return {backgroundImage: `url(${gem.bg})`};

        
    }

    render() {
    
        return (
            <div id="gem-screen" className="screen info-screen" style={this.getBgStyle()}>
                <div className="control-group">
                    {this.renderCloseButton()}
                    <div>
                        {this.rendeGem()}
                        
                    </div>
                </div>
               
          </div>
          );
        }
}

export default withRouter(GemScreen);