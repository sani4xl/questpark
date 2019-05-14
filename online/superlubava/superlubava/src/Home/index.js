import React, { Component } from 'react';
import { Route, Link, BrowserRouter as Router, withRouter } from 'react-router-dom';
import axios from 'axios';

import Qplogo from './../art/qplogo.png';
import LubavaLogo from './../art/lubava.png';
import gauntlet from './../art/infinity-gauntlet.png';

import SoleGemImg from './../art/sole_gem.png';
import TimeGemImg from './../art/time_gem.png';
import RealityGemImg from './../art/reality_gem.png';
import PowerGemImg from './../art/power_gem.png';
import SpaceGemImg from './../art/space_gem.png';
import MindGemImg from './../art/mind_gem.png';
import IronMan from './../art/iron-man.svg';

import './index.css';

const PROCESS_URL = `https://www.questpark.com.ua/game/lubava/backend/process.php`;
const GEMS = {
    'sole': { img: SoleGemImg, 'title': 'Камінь Душі' },
    'time': { img: TimeGemImg, 'title': 'Камінь Часу' },
    'reality': { img: RealityGemImg, 'title': 'Камінь Реальності' },
    'power': { img: PowerGemImg, 'title': 'Камінь Сили' },
    'space': { img: SpaceGemImg, 'title': 'Камінь Простору' },
    'mind': { img: MindGemImg, 'title': 'Камінь Розуму' },
};
const HIDE_GEM_TIMEOUT = 10 * 1000;

class Home extends Component {
    constructor() {
        super();
        this.state = {
            teamName: null,
        };
        this.handleSubmit = this.handleSubmit.bind(this);
        this.handleCheck = this.handleCheck.bind(this);
        this.gemClick = this.gemClick.bind(this);
    }

    handleCheck(event) {
        event.preventDefault();
        const data = new FormData(event.target);
        const code = data.get('code');
        if (!code) {
            alert("Будь ласка введіть код або шифр");
        }

        this.setState({ loading: true });

        axios.post(PROCESS_URL, {
            teamName: localStorage.getItem('teamName'),
            teamId: localStorage.getItem('teamId'),
            action: 'validate',
            code: code
        })
        .then(res => {
            this.setState({ loading: false });
            if (res.data) {
                if (res.data.found_gem) {
                    this.setState({
                        foundGem: GEMS[res.data.found_gem],
                        loading: false,
                    });
                    this.hideFoundGemTimeout = setTimeout(() => {
                        this.hideFoundGem();
                    }, HIDE_GEM_TIMEOUT);
                    return;
                }
            }
            this.showWrongAnswer();
            
        })
        .catch(() => {
            this.showWrongAnswer();
        });

        return false;
    }

    showWrongAnswer() {
        this.setState({wrong: true, loading: false, wrongLineClass: null });
        setTimeout(() => {
            this.setState({wrongLineClass: 'shrink'});
        }, 100);

        setTimeout(() => {
            this.setState({wrong: false});
        }, 2000);
    }

    hideFoundGem() {
       clearTimeout(this.hideFoundGemTimeout); 
       this.setState({foundGem: null});
    }

    handleSubmit(event) {
        event.preventDefault();
        const data = new FormData(event.target);
        const teamName = data.get('teamName');
        if (!teamName) {
            alert('Будь ласка вкажіть назву команди');
            return false;
        }

        axios.post(PROCESS_URL, {
            teamName: teamName,
            action: 'register',
        })
        .then(res => {
            if (res.data) {
                if (res.data.teamId) {
                    this.setState({
                        gems: res.data.gems, 
                        teamName: res.data.teamName,
                        teamId: res.data.teamId
                    });
                    localStorage.setItem('teamName', teamName);
                    localStorage.setItem('teamId', res.data.teamId);
                    alert("Вашу команду зараєстровано! Ласкаво просимо у гру!")
                }
            }
        });

        return false;
    }

    componentDidMount() {
        this.setState({ teamName: localStorage.getItem('teamName') });
        this.setState({ teamId: localStorage.getItem('teamId') });
    }

    renderRegistrationForm() {
        if (this.state.teamName) return;
        return <form onSubmit={this.handleSubmit} id="register-form">
            <div>Початок ваших пригод...</div>
            <div>
                <input placeholder="Назва команди" name="teamName" />
            </div>
            <button id="start-button" className="button">Зареєструватись</button>
        </form>;
    }

    gemClick() {
        this.hideFoundGem();
        this.props.history.push('/gauntlet');
    }

    renderFoundGem() {
        if (!this.state.foundGem) return;
        return <div>
            <div>Ви отримали <strong>{this.state.foundGem.title}</strong></div>
            <img onClick={this.gemClick} src={this.state.foundGem.img} alt={this.state.foundGem.title} id="found-gem" />
        </div>;
    }

    renderCheckForm() {
        if (this.state.foundGem || this.state.loading || this.state.wrong) return;
        return <div>
            <div id="check-text">
                Тільки секретні шифри та коди відкриють камені безкінечності...
            </div>
            <form onSubmit={this.handleCheck} id="check-form">
            <div>
                <input placeholder="Код, шифр, секретна фраза" name="code" autoComplete="off" />
            </div>
            <button id="check-button" className="button">Перевірити</button>
        </form>
        </div>;
    }

    renderLoading() {
        if  (!this.state.loading) return;
        return <div className="loader">Завантаження...</div>
    }

    renderWrongAnswer() {
        if  (!this.state.wrong) return;
        return <div id="wrong-answer">
            <div>Невдала спроба</div>
            <div id="wrong-line" className={this.state.wrongLineClass}></div>
        </div>;
    }

    renderGreetings() {
        if (!this.state.teamName) return;
        return <div id="intro-text">
            <div>
                Месники
                "{this.state.teamName}"
            </div>
            
            <div id="small-gauntlet-wrap">
                <Link to='/gauntlet'>
                    <img src={gauntlet} id="small-gauntlet" alt="Gauntlet" onLoad={this.onImgLoad} />
                </Link>
            </div>

            {this.renderFoundGem()}
            {this.renderWrongAnswer()}
            {this.renderLoading()}
            {this.renderCheckForm()}
        </div>
    }

    render() {
        return (
            <div id="home-grid">
                <div id="home-header">
                    <div>
                        <a href="http://lubava.ua" target="_blank">
                            <img src={LubavaLogo} alt="Lubava" className="qp-logo" />
                        </a>
                    </div>
                    <div>
                        <Link to='/menu'>
                            <img src={IronMan} alt="Settings" className="with-anim" id="settings-img" />
                        </Link>
                    </div>
                    <div>
                        <a href="https://questpark.com.ua" target="_blank">
                            <img src={Qplogo} alt="QuestPark" className="qp-logo" />
                        </a>
                    </div>
                </div>
                <div>
                    {this.renderRegistrationForm()}
                    {this.renderGreetings()}
                </div>
            </div>
        )
    }
}

export default withRouter(Home);