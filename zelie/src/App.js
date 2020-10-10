import React from 'react';
import { withRouter, Switch, Route, NavLink } from 'react-router-dom';

import './App.css';

import Music from './component/Music';
import Ingridients from './component/Ingridients/index';
import Main from './component/Main/index';
import Task from './component/Task/index';
import GemScreen from './component/GemScreen/index';
import Admin from './component/Admin/index';
import QR from './component/QR/index';


//function App() {
class App extends React.Component {

  constructor() {
    super();
    this.state = {
      started: false
    };

  }

  render() {
    //<Route path={`/course/:courseId`} component={CourseSelector} />
  return (
    <div className={`App ${this.state.started ? 'game': ''}`}>
      

      <Switch>
        
        <Route exact path='/ingridients' component={Ingridients} />
        <Route exact path='/task' component={Task} />
        <Route exact path='/master' component={Admin} />
        <Route path={`/gem/:gemName/new/:new`} component={GemScreen} />
        <Route path={`/gem/:gemName`} component={GemScreen} />
        <Route path={`/qr/:code`} component={QR} />
        
        <Route path='/' component={Main} />
        
      </Switch>

      <Music />
      

    </div>
  );
  }
}

export default withRouter(App);
