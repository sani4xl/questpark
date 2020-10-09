import React from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';
import GameInfo from '../helper/GameInfo';

import './index.css';

const axios = require('axios');
const REGISTER_URL = "https://hp.questpark.com.ua/api/index.php";
const gameInfo = new GameInfo();

class Task extends React.Component {


    constructor(props) {
        super(props);

        this.state = {
            teamName: '',
            taskText: 'Нету активных заданий'
        }
        
    }

    componentDidMount() {
        const currentTask = gameInfo.getTask();
        if (currentTask && currentTask.task) {
            this.setState({taskText: currentTask.task, icon: currentTask.icon});
        }
    }

    renderIcon() {
        if (!this.state.icon) {
            return;
        }
        return <div class="center">
                        <img className="gem-img" src={this.state.icon} />
                    </div>
    }
  
    render() {
    
        return (
            <div id="task-screen" className="screen info-screen">
                <div className="control-group">
                    <NavLink className="close-btn" to={`/`}></NavLink>
                    {this.renderIcon()}
                    <div className="task-text">
                        {this.state.taskText}
                    </div>
                </div>
               
          </div>
          );
        }
}

export default withRouter(Task);