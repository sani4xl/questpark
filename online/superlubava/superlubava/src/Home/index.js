import React, { Component } from 'react';
import { Route, Link, BrowserRouter as Router, withRouter } from 'react-router-dom';

import Qplogo from './../art/qplogo.png';
import LubavaLogo from './../art/lubava.png';
import gauntlet from './../art/infinity-gauntlet.png';

import './index.css';

class Home extends Component {
    constructor() {
        super();
        this.state = {
            teamName: null,
        };
        this.handleSubmit = this.handleSubmit.bind(this);
    }

    handleSubmit(event) {
        event.preventDefault();
        const data = new FormData(event.target);
        const teamName = data.get('teamName');
        localStorage.setItem('teamName', teamName);
        this.setState({ teamName });
        return false;
    }

    componentDidMount() {
        this.setState({ teamName: localStorage.getItem('teamName') });
    }

    renderRegistrationForm() {
        if (this.state.teamName) return;
        return <form onSubmit={this.handleSubmit}>
            <input placeholder="Назва команди" name="teamName" />
            <button>Зареєструватись</button>
        </form>;
    }

    renderGreetings() {
        if (!this.state.teamName) return;
        return <div>
            <div>
                Ласкаво просимо<br />комнада месників<br />
                "{this.state.teamName}"
            </div>
            <div>
                <br />
                Зберіть усі камні безкінечності у рукавицю...
            </div>
            <Link to='/gauntlet'>
                <img src={gauntlet} id="small-gauntlet" alt="Gauntlet" onLoad={this.onImgLoad} />
            </Link>
        </div>
    }

    render() {

        return (
            <div id="home-grid">
                <div>
                    <div>
                        <a href="http://lubava.ua" target="_blank">
                            <img src={LubavaLogo} alt="Lubava" class="qp-logo" />
                        </a>
                    </div>
                    <div>
                        <a href="https://questpark.com.ua" target="_blank">
                            <img src={Qplogo} alt="QuestPark" class="qp-logo" />
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