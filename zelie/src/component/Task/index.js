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
            this.setState({taskText: currentTask.task});
        }
    }

  
    render() {
    
        return (
            <div id="task-screen" className="screen info-screen">
                <div className="control-group">
                    <NavLink className="close-btn" to={`/`}></NavLink>
                    <div>
                        {this.state.taskText}
                    </div>
                </div>
               
          </div>
          );
        }
}

export default withRouter(Task);