import React, { Component } from 'react';
import { Route, Link, BrowserRouter as Router, withRouter } from 'react-router-dom';
import axios from 'axios';

import IronMan from './../art/iron-man.svg';
import Qplogo from './../art/qplogo.png';
import LubavaLogo from './../art/lubava.png';

import './index.css';

class Menu extends Component {
    constructor() {
        super();
        this.state = {
            teamName: null,
        };
        this.resetClick = this.resetClick.bind(this);
    }

    resetClick() {
        const agree = window.confirm("Досягнутий результат буде скинуто. Ви дійсно бажаєте вийти з гри?");
        if (agree) {
            localStorage.removeItem('teamName');
            localStorage.removeItem('teamId');
            this.props.history.push('/');
        }
    }

    render() {
        return (
            <div id="menu-grid">
                <div>
                    <img src={IronMan} alt="Settings" id="settings-img" />
                </div>
                <div>
                    <Link className="button default-btn" to='/'>На головну</Link>
                </div>
                <div>
                    <button className="button default-btn" id="reset-btn" onClick={this.resetClick}>Вийти з гри</button>
                </div>

                <div id="organizers">
                    <div>Організатори</div>
                    <div>
                        <a href="http://lubava.ua" target="_blank">
                            <img src={LubavaLogo} alt="Lubava" className="qp-logo" />
                            <br />
                            ТРЦ<br />"Любава"
                        </a>
                    </div>
                   
                    <div>
                        <a href="https://questpark.com.ua" target="_blank">
                            <img src={Qplogo} alt="QuestPark" className="qp-logo" />
                            <br />
                            Квест-Центр<br />"КвестПарк"
                        </a>
                    </div>
                </div>

            </div>
        )
    }
}

export default withRouter(Menu);