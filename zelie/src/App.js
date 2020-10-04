import React from 'react';
import logo from './logo.svg';
import './App.css';

import Music from './component/Music';
import Ingridients from './component/Ingridients/index';

//import Sound from 'react-sound';


//function App() {
class App extends React.Component {

  constructor() {
    super();
    this.state = {
      started: false
    };

  }

  renderStart() {
    if (this.state.started) {
      return;
    }
    return <button onClick={() => this.setState({started: true})} >Начать</button>
  }

  renderGame() {
    if (!this.state.started) {
      return;
    }

    return <div>
      <Ingridients />
      <Music />
    </div>
  }

  render() {
  return (
    <div className={`App ${this.state.started ? 'game': ''}`}>
      
      {this.renderStart()}
      {this.renderGame()}
      

    </div>
  );
  }
}

export default App;
