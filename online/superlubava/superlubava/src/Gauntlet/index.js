import React, { Component } from 'react';
import { Route, Link, BrowserRouter as Router, withRouter } from 'react-router-dom';
import gauntlet from './../art/infinity-gauntlet.png';
import SoleGemImg from './../art/sole_gem.png';
import TimeGemImg from './../art/time_gem.png';
import RealityGemImg from './../art/reality_gem.png';
import PowerGemImg from './../art/power_gem.png';
import SpaceGemImg from './../art/space_gem.png';
import MindGemImg from './../art/mind_gem.png';
import Snap from './../art/snap.png';
import './index.css';

const GLOVE_RATIO = 0.7066508313539192;

class Gauntlet extends Component {
    constructor() {
        super();
        this.state = {
            snapped: false
        };
        this.onImgLoad = this.onImgLoad.bind(this);
    }

    onImgLoad({ target: img }) {
        this.setState({
            gloveSize: {
                height: img.offsetHeight,
                width: img.offsetWidth,
                originalWidth: img.naturalWidth,
                originalHeight: img.naturalHeight,
                scale: img.offsetWidth / img.naturalWidth
            }
        });
    }

    renderSnap() {
        if (!this.state.snapped) return;
        return <div>
            <img src={Snap} id="snap" alt="Snap" />
        </div>
    }

    renderGauntlet() {
        if (this.state.snapped) return;
        const scale = this.state.gloveSize ? this.state.gloveSize.scale : 0;
        const soleGemStyle = {
            width: scale * 75,
            left: scale * 200,
            bottom: scale * 366,
        };

        const timeGemStyle = {
            width: scale * 50,
            left: scale * 435,
            bottom: scale * 327,
        };

        const realityGemStyle = {
            width: scale * 50,
            left: scale * 340,
            bottom: scale * 480,
        };

        const powerGemStyle = {
            width: scale * 43,
            left: scale * 276,
            bottom: scale * 492,
        };

        const spaceGemStyle = {
            width: scale * 50,
            left: scale * 140,
            bottom: scale * 515,
        };

        const mindGemStyle = {
            width: scale * 55,
            left: scale * 195,
            bottom: scale * 510,
        };

        return <div id="gauntlet-cell">
            <img src={gauntlet} id="gauntlet" alt="Gauntlet" onLoad={this.onImgLoad} />
            <img src={SoleGemImg} id="sole-gem" class="infinity-stone" style={soleGemStyle} />
            <img src={TimeGemImg} id="time-gem" class="infinity-stone" style={timeGemStyle} />
            <img src={RealityGemImg} id="reality-gem" class="infinity-stone" style={realityGemStyle} />
            <img src={PowerGemImg} id="power-gem" class="infinity-stone" style={powerGemStyle} />
            <img src={SpaceGemImg} id="space-gem" class="infinity-stone" style={spaceGemStyle} />
            <img src={MindGemImg} id="mind-gem" class="infinity-stone" style={mindGemStyle} />
        </div>
    }

    canSnap() {
        return true;
    }

    handleClick = () => {
        if (this.canSnap()) {
            this.setState({ snapped: true });
        }
    }

    renderClick() {
        if (this.state.snapped || !this.canSnap()) return;
        return <div>
            <button onClick={this.handleClick}>Клацнути!</button>
        </div>
    }

    render() {



        return (
            <div id="gauntlet-grid">
                {this.renderClick()}
                {this.renderGauntlet()}
                {this.renderSnap()}
            </div>
        )
    }
}

export default withRouter(Gauntlet);