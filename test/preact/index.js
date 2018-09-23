import './style';
import { Component, render, h } from '../../preact';

class Title extends Component {
	render() {
		return (<h1>{this.props.content}</h1>);
	}
}

class App extends Component {
	constructor() {
		super();
		this.state = {
			name: 'YHSPY',
			title: 'Introduction',
			x: 'x',
			y: 'y',
			z: 'z'
		};

		this.handleClick = this.handleClick.bind(this);
	}
	handleClick () {
		let update = this.state.name.split('').reverse().join('');
		this.setState({
			name: update
		});
	}
	shouldComponentUpdate (props, state) {
		return true;
	}
	/**
	// Compiled by babel;
	App.prototype.render = function render() {
		return Object(__WEBPACK_IMPORTED_MODULE_2__preact__["b"])(
			'div',
			{ onClick: this.handleClick },
			Object(__WEBPACK_IMPORTED_MODULE_2__preact__["b"])(
				'span',
				null,
				this.state.x,
				' + ',
				this.state.y,
				' + ',
				this.state.z
			),
			Object(__WEBPACK_IMPORTED_MODULE_2__preact__["b"])(Title, { content: this.state.title }),
			Object(__WEBPACK_IMPORTED_MODULE_2__preact__["b"])(
				'h3',
				null,
				'Hello, ',
				this.state.name,
				'!'
			)
		);
	};
	*/
	render() {
		return (
			<div onClick={this.handleClick}>
				<span>{this.state.x} + {this.state.y} + {this.state.z}</span>
				<Title content={this.state.title} />
				<h3>Hello, {this.state.name}!</h3>
			</div>
		);
	}
}

render(<App />, document.body);
