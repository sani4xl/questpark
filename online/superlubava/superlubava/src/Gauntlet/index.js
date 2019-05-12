import React, { Component } from 'react';
import { Route, Link, BrowserRouter as Router, withRouter } from 'react-router-dom';

import axios from 'axios';

import gauntlet from './../art/infinity-gauntlet.png';
import SoleGemImg from './../art/sole_gem.png';
import TimeGemImg from './../art/time_gem.png';
import RealityGemImg from './../art/reality_gem.png';
import PowerGemImg from './../art/power_gem.png';
import SpaceGemImg from './../art/space_gem.png';
import MindGemImg from './../art/mind_gem.png';
import Snap from './../art/snap.png';
import './index.css';

class Gauntlet extends Component {
    constructor() {
        super();
        this.state = {
            snapped: false
        };
        this.onImgLoad = this.onImgLoad.bind(this);
    }

    componentDidMount() {
        axios.post(`https://questpark.com.ua/game/superlubava/backend/process.php`, {
            teamName: "hello",
        })
        .then(res => {
            if (res.data) {
                this.setState({gems: res.data.gems})
            }
        })  
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

    isNoGem(gem) {
        return !this.state.gems || !this.state.gems[gem];
    }

    renderSoleGem() {
        if (this.isNoGem('sole')) return;
        const scale = this.state.gloveSize ? this.state.gloveSize.scale : 0;
        const soleGemStyle = {
            width: scale * 75,
            left: scale * 200,
            bottom: scale * 366,
        };
        return <img src={SoleGemImg} id="sole-gem" className="infinity-stone" style={soleGemStyle} />;
    }

    renderTimeGem() {
        if (this.isNoGem('time')) return;
        const scale = this.state.gloveSize ? this.state.gloveSize.scale : 0;
        const timeGemStyle = {
            width: scale * 50,
            left: scale * 435,
            bottom: scale * 327,
        };
        return <img src={TimeGemImg} id="time-gem" className="infinity-stone" style={timeGemStyle} />;
    }

    renderRealityGem() {
        if (this.isNoGem('reality')) return;
        const scale = this.state.gloveSize ? this.state.gloveSize.scale : 0; 
        const realityGemStyle = {
            width: scale * 50,
            left: scale * 340,
            bottom: scale * 480,
        };
        return  <img src={RealityGemImg} id="reality-gem" className="infinity-stone" style={realityGemStyle} />;
    }

    renderPowerGem() {
        if (this.isNoGem('power')) return;
        const scale = this.state.gloveSize ? this.state.gloveSize.scale : 0;
        const powerGemStyle = {
            width: scale * 43,
            left: scale * 276,
            bottom: scale * 492,
        };
        return <img src={PowerGemImg} id="power-gem" className="infinity-stone" style={powerGemStyle} />;
    }

    renderSpaceGem() {
        if (this.isNoGem('space')) return;
        const scale = this.state.gloveSize ? this.state.gloveSize.scale : 0;
        const spaceGemStyle = {
            width: scale * 50,
            left: scale * 140,
            bottom: scale * 515,
        };
        return <img src={SpaceGemImg} id="space-gem" className="infinity-stone" style={spaceGemStyle} />;
    }

    renderMindGem() {
        if (this.isNoGem('mind')) return;
        const scale = this.state.gloveSize ? this.state.gloveSize.scale : 0;
        const mindGemStyle = {
            width: scale * 55,
            left: scale * 195,
            bottom: scale * 510,
        };
        return <img src={MindGemImg} id="mind-gem" className="infinity-stone" style={mindGemStyle} />;
    }

    renderGauntlet() {
        if (this.state.snapped) return;
        return <div id="gauntlet-cell">
            <img src={gauntlet} id="gauntlet" alt="Gauntlet" onLoad={this.onImgLoad} />
            {this.renderSoleGem()}
            {this.renderMindGem()}
            {this.renderPowerGem()}
            {this.renderRealityGem()}
            {this.renderTimeGem()}
            {this.renderSpaceGem()}
        </div>
    }

    canSnap() {
        return this.state.gems && Object.keys(this.state.gems).length === 6;
    }

    handleClick = () => {
        if (this.canSnap()) {
            this.setState({ snapped: true });
        }
    }

    renderClick() {
        if (this.state.snapped) return;
        return <div>
            <button onClick={this.handleClick} disabled={!this.canSnap()}>Клацнути!</button>
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